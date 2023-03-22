use axum::{
    http::Response,
    response::{Html, IntoResponse},
};

pub async fn root() -> impl IntoResponse {
    let markup = tokio::fs::read_to_string("../frontend/index.html")
        .await
        .unwrap();

    Html(markup)
}

pub async fn js() -> impl IntoResponse {
    let js = tokio::fs::read_to_string("../frontend/index.js")
        .await
        .unwrap();

    Response::builder()
        .header("content-type", "application/javascript;charset=utf8")
        .body(js)
        .unwrap()
}

pub async fn lucky_number() -> impl IntoResponse {
    "42"
}
