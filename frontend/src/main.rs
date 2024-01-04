use leptos::*;

mod home;
use home::Home;

fn main() {
    mount_to_body(|| view! {
        <Home />
    })
}
