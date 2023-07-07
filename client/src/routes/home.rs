use crate::components::input::InputField;
use crate::types::meal::Meal;

use gloo_console::log;
use web_sys::HtmlInputElement;
use yew::prelude::*;

pub enum Msg {
    Add(Meal),
    Remove(Meal),
}

pub struct Home {
    meals: Vec<Meal>,
}

impl Component for Home {
    type Message = Msg;
    type Properties = ();

    fn create(_ctx: &Context<Self>) -> Self {
        Self { meals: Vec::new() }
    }

    fn update(&mut self, _ctx: &Context<Self>, msg: Self::Message) -> bool {
        match msg {
            Msg::Add(meal) => self.meals.push(meal),
            Msg::Remove(meal) => {
                let meals = self
                    .meals
                    .drain(..)
                    .filter(|m| meal.name == m.name)
                    .collect();
                self.meals = meals;
            }
        }

        true
    }

    fn view(&self, _ctx: &Context<Self>) -> Html {
        html! {
            <div>
                <Form />
                <p> { format!("Contains: {}", self.meals.len())} </p>

                <ul class="item-list">
                    { self.meals.iter().map(|meal| format!("{}", meal.name)).collect::<Html>() }
                </ul>
            </div>
        }
    }
}

#[derive(Debug)]
pub struct MealForm {
    pub name: String,
}

#[function_component(Form)]
pub fn meal_form() -> Html {
    let name = use_node_ref();

    let onsubmit = {
        let name = name.clone();

        Callback::from(move |_: SubmitEvent| {
            let meal_name: String = match name.clone().cast::<HtmlInputElement>() {
                Some(element) => String::from(element.value()),
                None => String::from(""),
            };

            log!(&meal_name);
            let meal_form = MealForm { name: meal_name };
            log!(format!("{:?}", meal_form));
        })
    };

    html! {
        <main>
            <h1> { "Submit Meal" } </h1>
            <form {onsubmit}>
                <InputField input_node_ref={name} label={"Meal".to_string()} name={"meal".to_string()} field_type={"text"} />
            </form>
        </main>
    }
}
