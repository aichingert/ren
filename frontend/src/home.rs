use leptos::{html::Input, *};

const ENTER_KEY: u32 = 13;
const _ESCAPE_KEY: u32 = 27;

#[component]
pub fn Home() -> impl IntoView {
    view! {
        <div class="home">
            <div class="header">
                <h1> Cooking-Center </h1>
            </div>

            <div class="recipes">
                <div>
                   <h2> Recipe x </h2>
                   <p> Description lorum ipsum </p>
                   <p> IMAGE </p>
                   <p> IMAGE </p>
                   <p> IMAGE </p>
                </div>
                
                <h2> Recipe x </h2>
                <h2> Recipe x </h2>
                <h2> Recipe x </h2>
                <h2> Recipe x </h2>
                <h2> Recipe x </h2>
                <h2> Recipe x </h2>
            </div>

            <div class="options">
                <p> Add </p>
                <p> Add </p>
                <p> Add </p>
                <p> Add </p>
                <p> Add </p>
                <p> Add </p>
            </div>
        </div>
    }
}
