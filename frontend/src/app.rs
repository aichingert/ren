use yew::prelude::*;
use yew_router::prelude::*;

use crate::pages::{
    home::Home,
    timetable::Timetable,
    page_not_found::PageNotFound
};

#[derive(Routable, Clone, Debug, PartialEq, Eq)]
pub enum Route {
    #[at("/")]
    Home,
    #[at("/timetable")]
    Timetable,
    #[not_found]
    #[at("/404")]
    NotFound
}

#[function_component]
pub fn App() -> Html {
    html! {
        <BrowserRouter>
            <main>
                <Switch<Route> render={switch} />
            </main>

            <h1> { "App" } </h1>
        </BrowserRouter>
    }
}

fn switch(routes: Route) -> Html {
    match routes {
        Route::Home => html! { <Home /> },
        Route::Timetable => html! { <Timetable /> },
        Route::NotFound => html! { <PageNotFound /> }
    }
}
