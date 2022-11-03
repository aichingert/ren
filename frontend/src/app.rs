use yew::prelude::*;
use yew_router::prelude::*;

use crate::pages::{
    home::Home,
    timetable::Timetable,
    note::Note,
    page_not_found::PageNotFound
};
use crate::components::nav::Nav;

#[derive(Routable, Clone, Debug, PartialEq, Eq)]
pub enum Route {
    #[at("/")]
    Home,
    #[at("/timetable")]
    Timetable,
    #[at("/note")]
    Note,
    #[not_found]
    #[at("/404")]
    NotFound
}

#[function_component]
pub fn App() -> Html {
    html! {
        <BrowserRouter>
            <Nav />

            <main>
                <Switch<Route> render={switch} />
            </main>

            <footer> { "footer?" } </footer>
        </BrowserRouter>
    }
}

fn switch(routes: Route) -> Html {
    match routes {
        Route::Home => html! { <Home /> },
        Route::Timetable => html! { <Timetable /> },
        Route::Note => html! { <Note /> },
        Route::NotFound => html! { <PageNotFound /> }
    }
}
