use yew::prelude::*;

use crate::components::text_input::TextInput;

#[derive(Properties, Clone, Debug, PartialEq, Eq)]
pub struct Props;


pub struct Note {
    log: String
}

pub enum Msg {
    Write(String)
}

impl Note {
    fn default() -> Self {
        Self {
            log: String::new()
        }
    }
}

impl Component for Note {
    type Message = Msg; 
    type Properties = ();

    fn create(_ctx: &yew::Context<Self>) -> Self {
        Note::default()
    }

    fn update(&mut self, _ctx: &Context<Self>, msg: Self::Message) -> bool {
        match msg {
            Msg::Write(value) => self.log = value
        }

        true
    }

    fn view(&self, ctx: &Context<Self>) -> Html {
        let on_change = ctx.link().callback(Msg::Write);

        html! {
            <> 
                <h1> { "Notes" } </h1>

                <div>
                    <TextInput {on_change} value={self.log.clone()} />
                </div>
            </>
        }   
    }

}

