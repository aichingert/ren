CREATE TABLE IF NOT EXISTS recipe (
  id INT2 NOT NULL,
  name TEXT NOT NULL,
  image TEXT NOT NULL,
  PRIMARY KEY (id)
);

CREATE TABLE IF NOT EXISTS history (
  id DATE NOT NULL,
  recipe_id INT2 NOT NULL,
  PRIMARY KEY (id),
  FOREIGN KEY (recipe_id) REFERENCES recipe(id)
);
