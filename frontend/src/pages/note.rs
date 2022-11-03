use yew::prelude::*;

#[derive(Properties, Clone, Debug, PartialEq, Eq)]
pub struct Props;

#[function_component]
pub fn Note(_props: &Props) -> Html {
    html! {
        <>
            <h1> { "Notes" } </h1>
        </>
    }
}
