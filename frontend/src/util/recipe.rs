pub struct Recipes(pub Vec<Recipe>);

impl Recipes {
    pub fn new() -> Self {
        Self(Vec::new())
    }

    pub fn add(&mut self, recipe: Recipe) {
        self.0.push(recipe);
    }

    pub fn remove(&mut self, idx: usize) {
        self.0.remove(idx);
    }
}

#[derive(Clone)]
pub struct Recipe {
    pub name: String,
    pub image: String,
    pub stars: u8,
}

impl Recipe {
    pub fn new(name: String, stars: u8) -> Self {
        Self {
            name,
            image: "./butter_chicken.jpg".to_string(),
            stars,
        }
    }
}

