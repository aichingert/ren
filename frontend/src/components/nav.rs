use yew::prelude::*;
use yew_router::prelude::*;

use crate::app::Route;

#[function_component]
pub fn Nav() -> Html {
    html! {
        <>
            <nav class="navbar">
                <div class="navbar-item">
                    <Link<Route> classes={classes!("navbar-item", "navbar-header")} to={Route::Home}>
                        { "≣ Notes" }
                    </Link<Route>>
                    <Link<Route> classes={classes!("navbar-item")} to={Route::Home}>
                        { "Home" }
                    </Link<Route>>
                    <Link<Route> classes={classes!("navbar-item")} to={Route::Note}>
                        { "Note" }
                    </Link<Route>>
                    <Link<Route> classes={classes!("navbar-item")} to={Route::Timetable}>
                        { "Timetable" }
                    </Link<Route>>
                </div>
            </nav>
        </>
    }
}
