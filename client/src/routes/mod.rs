use yew::prelude::*;
use yew_router::prelude::*;

#[derive(Routable, Debug, Clone, PartialEq, Eq)]
pub enum AppRoute {
    #[at("/login")]
    Login,
    #[at("/")]
    Home,
    #[not_found]
    #[at("/404")]
    NotFound,
}

pub fn switch(route: AppRoute) -> Html {
    match route {
        AppRoute::Login => html! { <h1> { "Login" } </h1> },
        AppRoute::Home => html! { <h1> { "Homie" } </h1> },
        AppRoute::NotFound => html! { <h1> { "Not found" } </h1> },
    }
}
