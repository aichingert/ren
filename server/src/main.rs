use actix_web::{get, web, App, HttpServer, Responder};
use sqlx::postgres::PgPool;
use std::net::TcpListener;

mod db;
use db::basic::Database;

const PORT: usize = 4200;

#[get("/index")]
async fn index(pool: web::Data<PgPool>) -> impl Responder {
    let row: (i64,) = sqlx::query_as("insert into recipe (name) values ($1) returning id")
        .bind("chicken filet")
        .fetch_one(pool.get_ref())
        .await
        .unwrap();

    format!("{:?}", row)
}

#[tokio::main]
async fn main() -> Result<(), std::io::Error> {
    let db: Database = Database::new();
    let pool = db.connect().await.unwrap();
    db.setup(&pool).await.unwrap();

    let listener: TcpListener = TcpListener::bind(format!("127.0.0.0:{PORT}"))?;

    HttpServer::new(move || App::new().data(pool.clone()).service(index))
        .listen(listener)?
        .run()
        .await
}

/*

println!("{:?}", row);

let query = sqlx::query_as::<_, Recipe>("SELECT id, name FROM RECIPE");
let recipes = query.fetch_all(&pool).await?;
println!("{:?}", recipes[0].id);
*/
