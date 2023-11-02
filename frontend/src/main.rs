use leptos::{html::Input, *}; 

const ENTER_KEY: u32 = 13;
const _ESCAPE_KEY: u32 = 27;

struct Recipes(pub Vec<Recipe>);

impl Recipes {
    pub fn new() -> Self {
        Self(Vec::new())
    }

    pub fn add(&mut self, recipe: Recipe) {
        self.0.push(recipe);
    }
}

#[derive(Clone)]
struct Recipe {
    name: String,
    stars: u8,
}

impl Recipe {
    pub fn new(name: String, stars: u8) -> Self {
        Self {
            name,
            stars,
        }
    }
}

#[component]
pub fn Home() -> impl IntoView {
    let (recipes, set_recipes) = create_signal(Recipes::new());
    let mut counter = 0;

    provide_context(set_recipes);

    let input_ref = create_node_ref::<Input>();
    let add_recipe = move |ev: web_sys::KeyboardEvent| {
        let input = input_ref.get().unwrap();
        ev.stop_propagation();

        if ev.key_code() == ENTER_KEY {
            let name = input.value().trim().to_string();

            if !name.is_empty() {
                set_recipes.update(|r| r.add(Recipe::new(name, counter + 1)));
                counter += 1;
                input.set_value("");
            }
        }
    };

    let recipes = move || {
        recipes.with(|recipes| recipes.0.to_vec())
    };

    view! {
        <main>
            <div class="area">
                <h1> Recipes </h1>
            </div>

            <input
                placeholder="new recipe"
                on:keydown=add_recipe
                node_ref=input_ref
            />

            <ul>
                <For
                    each=recipes
                    key=|recipe| recipe.stars
                    let:recipe
                >
                    <p>{ recipe.name }</p>
                </For>
            </ul>
        </main>
    }
}

fn main() {
    mount_to_body(|| view! {
        <Home />
    })
}
