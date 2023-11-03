use leptos::*;
use crate::Recipe;

#[component]
pub fn RecipeView(recipe: Recipe) -> impl IntoView {
    view! {
        <div class="recipe" >
            <h2> Recipe { recipe.stars } </h2>
            <p> { recipe.name } </p>
        </div>
    }
}
