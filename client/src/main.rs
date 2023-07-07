mod components;
mod routes;
mod types;
use routes::{switch, AppRoute};

use yew::prelude::*;
use yew_router::prelude::*;

#[function_component(App)]
fn app() -> Html {
    html! {
        <HashRouter>
            <Switch<AppRoute> render={switch} />
        </HashRouter>
    }
}

fn main() {
    yew::Renderer::<App>::new().render();
}
