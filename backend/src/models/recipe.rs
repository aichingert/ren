use serde::{Serialize, Deserialize};

#[derive(Serialize, sqlx::FromRow)]
pub struct Recipe {
    pub id: i16,
    pub name: String,
    pub image: String,
}

#[derive(sqlx::FromRow)]
pub struct Id {
    pub id: i16,
}

#[derive(Deserialize)]
pub struct CreateRecipe {
    pub name: String,
    pub image: String,
}

impl Recipe {
    pub async fn get_by_id(id: i16, pool: &sqlx::PgPool) -> Result<Self, sqlx::Error> {
        return sqlx::query_as!(Self, "SELECT * FROM recipe WHERE id = $1", id)
            .fetch_one(pool)
            .await
    }
}
