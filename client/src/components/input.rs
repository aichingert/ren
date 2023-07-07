use yew::prelude::*;

#[derive(Clone, PartialEq, Properties)]
pub struct InputFieldProps {
    pub label: String,
    pub field_type: String,
    pub name: String,
    pub input_node_ref: NodeRef,
}

#[function_component(InputField)]
pub fn input_field(props: &InputFieldProps) -> Html {
    let InputFieldProps {
        label,
        field_type,
        name,
        input_node_ref,
    } = props;

    html! {
        <label for="cautios-input">
            { label }
            <input
                type={field_type.clone()}
                name={name.clone()}
                ref={input_node_ref.clone()}
            />
        </label>
    }
}
