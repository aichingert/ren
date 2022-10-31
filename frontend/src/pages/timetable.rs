use yew::prelude::*;

#[derive(Clone, Debug, Eq, PartialEq, Properties)]
pub struct Props;

#[function_component]
pub fn Timetable(_props: &Props) -> Html {
    html! {
        <>
            <h1> { "Timetable" } </h1>
        </>
    }
}
