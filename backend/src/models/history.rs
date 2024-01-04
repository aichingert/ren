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
    pub id: NaiveDate,
    pub recipe_id: i16,
}

pub struct Id {
    pub id: NaiveDate,
}

#[derive(Deserialize)]
pub struct CreateHistory {
    pub year: i32,
    pub month: u32,
    pub day: u32,
    pub recipe_id: i16,
}

impl History {
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
}
