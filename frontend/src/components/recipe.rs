use leptos::*;
use crate::Recipe;

#[component]
pub fn RecipeView(recipe: Recipe) -> impl IntoView {

    view! {
        <div class="recipe" >
            <div style={"position:relative; text-align: left"}>
                <p>Fr</p>
            </div>
            <h2> Recipe { recipe.stars } </h2>
            <p> { &recipe.name } </p>
            <img src={ &recipe.image } />
        </div>
        <div class="recipe" >
            <div style={"position:relative; text-align: left"}>
                <p>Do</p>
            </div>
            <h2> Recipe { recipe.stars } </h2>
            <p> { &recipe.name } </p>
            <p> { &recipe.image } </p>
        </div>
    }
}
