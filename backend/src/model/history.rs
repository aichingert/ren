use axum::{
    body::Body,
    extract::State,
    response::{IntoResponse, Response},
    http::StatusCode,
    Json,
};
use serde::{Serialize, Deserialize};
use sqlx::{types::chrono::NaiveDate, PgPool};

use crate::model::recipe::Recipe;

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

#[derive(Debug)]
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

        let result = sqlx::query_as!(
                Id, 
                "INSERT INTO History ( id, recipe_id ) VALUES ( $1, $2 ) RETURNING id", 
                date, input.recipe_id
            )
            .fetch_one(&pool)
            .await.unwrap();

        println!("{:?}", result);

        StatusCode::OK.into_response()
    }
}

