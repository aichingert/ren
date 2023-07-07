pub struct Meal {
    pub name: String,
}

impl Meal {
    pub fn new(name: String) -> Self {
        Self { name }
    }
}
