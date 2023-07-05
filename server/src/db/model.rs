use sqlx::FromRow;

#[derive(FromRow)]
pub struct Recipe {
    pub id: i64,
    pub name: String,
}
