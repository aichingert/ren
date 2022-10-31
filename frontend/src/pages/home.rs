use yew::prelude::*;

#[derive(Clone, Debug, Eq, PartialEq, Properties)]
pub struct Props;

#[function_component]
pub fn Home(_props: &Props) -> Html {
    html! {
        <>
            <h1> { "Previous notes" } </h1>
        </>
    }
}

