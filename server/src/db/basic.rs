use sqlx::postgres::PgPoolOptions;
use sqlx::PgPool;

pub struct Database {
    url: String,
}

impl Database {
    pub fn new() -> Self {
        let url: String = dotenvy::var("DATABASE_URL").expect("No database");

        Self { url }
    }

    pub async fn connect(&self) -> Result<PgPool, Box<dyn std::error::Error>> {
        let connection: PgPool = PgPoolOptions::new()
            .max_connections(20)
            .connect(&self.url)
            .await?;

        Ok(connection)
    }

    pub async fn setup(&self, pool: &PgPool) -> Result<(), Box<dyn std::error::Error>> {
        if let Ok(result) = sqlx::query(
            r#"
                DROP TABLE recipe;
            "#,
        )
        .execute(pool)
        .await
        {
            println!("TABLES DROPED: {:?}", result);
        };

        sqlx::query(
            r#"CREATE TABLE IF NOT EXISTS recipe (
                    id bigserial,
                    name TEXT NOT NULL
                )"#,
        )
        .execute(pool)
        .await?;

        Ok(())
    }
}
