use axum::{
    Router, Server, 
    routing::get
};
use std::error::Error;
use sqlx::Row;

const URL: &'static str = "postgress://app:app@localhost:5432/db";

mod routes;

use routes::{root, js, lucky_number};

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let router = Router::new()
        .route("/", get(root))
        .route("/index.js", get(js))
        .route("/api/lucky", get(lucky_number));

    let server = Server::bind(&"127.0.0.1:8080".parse().unwrap())
        .serve(router.into_make_service());

    let pool = sqlx::postgres::PgPool::connect(URL).await?;
    let res = sqlx::query("SELECT 1 + 1 as sum")
        .fetch_one(&pool)
        .await?;

    let sum: i32 = res.get("sum");
    println!("1 + 1 = {sum}");

    println!("{:?}", server.local_addr());

    server
        .await
        .unwrap();

    Ok(())
}
