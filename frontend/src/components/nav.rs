use yew::prelude::*;
use yew_router::prelude::*;

use crate::app::Route;

#[function_component]
pub fn Nav() -> Html {
    html! {
        <>
            <nav>
                <div>
                    <h1> { "Notes" } </h1>
                </div>
                <div>
                    <Link<Route> to={Route::Home}>
                        { "Home" }
                    </Link<Route>>
                    <Link<Route> to={Route::Timetable}>
                        { "Timetable" }
                    </Link<Route>>
                </div>
            </nav>
        </>
    }
}
