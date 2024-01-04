use axum::{
    routing::get,
    Router,
};
use sqlx::postgres::PgPoolOptions;

mod routes;
mod models;

use models::{
    recipe::Recipe,
    history::History,
};

async fn root() -> &'static str {
    "Health check!"
}

#[tokio::main]
async fn main() -> Result<(), sqlx::Error> {
    let pool = PgPoolOptions::new()
        .max_connections(5)
        .connect("postgresql://pg:pg@localhost:5432/cc").await?;

    let history_routes = Router::new()
        .route("/", get(History::get_all).post(History::new))
        .route("/:id", get(History::get_by_id_api));

    let recipe_routes = Router::new()
        .route("/", get(Recipe::get_all).post(Recipe::new))
        .route("/:id", get(Recipe::get_by_id_api));

    let routes = Router::new()
        .nest("/recipes", recipe_routes)
        .nest("/histories", history_routes);

    let app = Router::new()
        .route("/", get(root))
        .nest("/api", routes)
        .with_state(pool);

    let listener = tokio::net::TcpListener::bind("0.0.0.0:3000").await.unwrap();
    axum::serve(listener, app).await.unwrap();

    Ok(())
}
