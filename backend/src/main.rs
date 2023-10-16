use sqlx::{SqlitePool, sqlite::SqlitePoolOptions};
use axum_session::{SessionConfig, SessionSqlitePool, SessionStore};
use axum_session_auth::*;

mod entity;
use entity::Member;

async fn connect_to_database() -> SqlitePool {
    SqlitePoolOptions::new()
        .connect("sqlite:Cooking.db")
        .await
        .expect("Could not make pool.")
}

#[tokio::main]
async fn main() {
    let pool = connect_to_database().await;

    let session_config = SessionConfig::default().with_table_name("test_table");
    let auth_config = AuthConfig::<i64>::default().with_anonymous_user_id(Some(1));

    let session_store = 
        SessionStore::<SessionSqlitePool>::new(Some(pool.clone().into()), session_config)
        .await
        .unwrap();

    Member::create_member_tables(&pool).await;
}
