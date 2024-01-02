pub mod recipe;

#[derive(sqlx::FromRow)]
pub struct Id {
    id: i16,
}
