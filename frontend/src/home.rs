use leptos::{html::Input, *};

const ENTER_KEY: u32 = 13;
const _ESCAPE_KEY: u32 = 27;

#[component]
pub fn Home() -> impl IntoView {
    view! {
        <div class="home">
            <div class="header">
                <span style="color: rgb(137, 221, 255)">"<"</span>
                <span style="color: rgb(240, 113, 120)">"Cooking-Center"</span>
                <span style="color: rgb(137, 221, 255)">"/>"</span>
            </div>

            <div class="recipes">
                <div>
                    <span style="color: rgb(137, 221, 255)">"<"</span>
                    <span style="color: rgb(195, 232, 141)">"Recipe x"</span>
                    <span style="color: rgb(137, 221, 255)">"/>"</span>

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
