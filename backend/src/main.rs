use axum::{
    Router, Server, 
    routing::get
};

mod routes;

use routes::{root, js};

#[tokio::main]
async fn main() {

    let router = Router::new()
        .route("/", get(root))
        .route("/index.js", get(js));

    let server = Server::bind(&"127.0.0.1:8080".parse().unwrap())
        .serve(router.into_make_service());

    println!("{:?}", server.local_addr());;

    server
        .await
        .unwrap();
}
