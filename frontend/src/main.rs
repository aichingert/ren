use leptos::*;

mod util;
use util::recipe::{Recipes, Recipe};

mod components;
use components::{Home,RecipeView};

fn main() {
    mount_to_body(|| view! {
        <Home />
    })
}
