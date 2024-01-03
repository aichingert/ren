use axum::{
    body::Body,
    extract::{Path, State},
    response::{IntoResponse, Response},
    http::StatusCode,
    Json
};
use sqlx::PgPool;

use crate::models::recipe::{
    Recipe,
    Id,
    CreateRecipe,
};

impl Recipe {
    pub async fn new(State(pool): State<PgPool>, Json(input): Json<CreateRecipe>) -> Response<Body> {
        let id = sqlx::query_as!(
                Id, 
                "INSERT INTO recipe (name, image) VALUES ($1, $2) RETURNING id", 
                input.name, input.image
            )
            .fetch_one(&pool)
            .await;

        match id {
            Ok(Id { id }) => (StatusCode::CREATED, Json(Recipe::get_by_id(id, &pool).await.unwrap())).into_response(),
            Err(_) => StatusCode::INTERNAL_SERVER_ERROR.into_response(),
        }
    }

    pub async fn get_by_id_api(State(pool): State<PgPool>, Path(id): Path<String>) -> Response<Body> {
        let id = match id.parse::<i16>() {
            Ok(val) => val,
            Err(_) => return StatusCode::BAD_REQUEST.into_response()
        };

        match Recipe::get_by_id(id, &pool).await {
            Ok(recipe) => Json(recipe).into_response(),
            Err(_) => StatusCode::NOT_FOUND.into_response(),
        }
    }

    pub async fn get_all(State(pool): State<PgPool>) -> Response<Body> {
        let result = sqlx::query_as!(Self, "SELECT * FROM recipe")
            .fetch_all(&pool)
            .await;

        match result {
            Ok(recipes) => (StatusCode::OK, Json(recipes)).into_response(),
            Err(_) => StatusCode::INTERNAL_SERVER_ERROR.into_response(),
        }
    }
}
