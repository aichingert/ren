use axum::{
    body::Body,
    extract::{State, Path},
    response::{IntoResponse, Response},
    http::StatusCode,
    Json,
};
use serde::{Serialize, Deserialize};
use sqlx::{types::chrono::NaiveDate, PgPool};

use crate::models::recipe::Recipe;

#[derive(Serialize)]
pub struct History {
    id: String,
    recipe: Recipe,
}

#[derive(sqlx::FromRow)]
pub struct DbHistory {
    id: NaiveDate,
    recipe_id: i16,
}

pub struct Id {
    id: NaiveDate,
}

#[derive(Deserialize)]
pub struct CreateHistory {
    year: i32,
    month: u32,
    day: u32,
    recipe_id: i16,
}

impl History {
    pub async fn new(State(pool): State<PgPool>, Json(input): Json<CreateHistory>) -> Response<Body> {
        let date = if let Some(date) = NaiveDate::from_ymd_opt(input.year, input.month, input.day) {
            date
        } else {
            return StatusCode::BAD_REQUEST.into_response()
        };

        let date = match sqlx::query_as!(
                Id, 
                "INSERT INTO history ( id, recipe_id ) VALUES ( $1, $2 ) RETURNING id", 
                date, input.recipe_id
            )
            .fetch_one(&pool)
            .await 
        {
            Ok(Id { id }) => id,
            Err(_) => return StatusCode::INTERNAL_SERVER_ERROR.into_response(),
        };

        if let Ok(history) = Self::get_by_id(date, &pool).await {
            (StatusCode::OK, Json(history)).into_response()
        } else {
            StatusCode::INTERNAL_SERVER_ERROR.into_response()
        }
    }

    pub async fn convert_db_history(db: DbHistory, pool: &PgPool) -> Result<Self, sqlx::Error> {
        let recipe = Recipe::get_by_id(db.recipe_id, pool)
            .await?;

        Ok(Self {
            id: db.id.to_string(),
            recipe,
        })
    } 

    pub async fn get_by_id(id: NaiveDate, pool: &PgPool) -> Result<Self, sqlx::Error> {
        let db = sqlx::query_as!(DbHistory, "SELECT * FROM history WHERE id = $1", id)
            .fetch_one(pool)
            .await?;

        Self::convert_db_history(db, pool).await
    }

    pub async fn get_by_id_api(State(pool): State<PgPool>, Path(id): Path<String>) -> Response<Body> {
        let date = match id.split('-').map(str::parse::<u32>).collect::<Result<Vec<_>, _>>() {
            Ok(date) => if date.len() != 3 { return StatusCode::BAD_REQUEST.into_response() } else { date },
            Err(_) => return StatusCode::BAD_REQUEST.into_response(),
        };

        if let Some(date) = NaiveDate::from_ymd_opt(date[0] as i32, date[1], date[2]) {
            if let Ok(history) = Self::get_by_id(date, &pool).await {
                return (StatusCode::OK, Json(history)).into_response();
            } else {
                return StatusCode::NOT_FOUND.into_response();
            }
        }

        StatusCode::BAD_REQUEST.into_response()
    }

    pub async fn get_all(State(pool): State<PgPool>) -> Response<Body> {
        let histories = sqlx::query_as!(DbHistory, "SELECT * FROM history")
            .fetch_all(&pool)
            .await;

        match histories {
            Ok(xs) => {
                let mut histories = Vec::new();

                for db in xs {
                    histories.push(match Self::convert_db_history(db, &pool).await {
                        Ok(hi) => hi,
                        Err(_) => return StatusCode::INTERNAL_SERVER_ERROR.into_response(),
                    });
                }

                (StatusCode::OK, Json(histories)).into_response()
            }
            Err(_) => StatusCode::INTERNAL_SERVER_ERROR.into_response(),
        }
    }
}
