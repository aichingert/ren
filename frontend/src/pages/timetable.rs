use yew::prelude::*;

#[dervie(Clone, Debug, Eq, PartialEq, Properties)]
pub struct Props;

#[function_component]
fn Timetable(props: &Props) -> Html {
    html! {
        <>
            <h1> { "Timetable" } </h1>
        </>
    }
}
