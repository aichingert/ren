use sqlx::{SqlitePool, FromRow};
use axum_session_auth::*;
use async_trait::async_trait;

pub struct Member {
    id: u32,
}

#[derive(FromRow, Clone)]
pub struct SqlMember {
    pub id: u32,
}

impl SqlMember {
    pub fn into_member(self) -> Member {
        Member {
            id: self.id
        }
    }
}

impl Default for Member {
    fn default() -> Self {
        Self {
            id: 1,
        }
    }
}

#[async_trait]
impl Authentication<Member, i64, SqlitePool> for Member {
    async fn load_user(member_id: i64, pool: Option<&SqlitePool>) -> Result<Member, anyhow::Error> {
        let pool = pool.unwrap();

        Member::get_member(member_id, pool)
            .await
            .ok_or_else(|| anyhow::anyhow!("Could not load member"))
    }

    fn is_authenticated(&self) -> bool {
        true
    }

    fn is_active(&self) -> bool {
        true
    }

    fn is_anonymous(&self) -> bool {
        false
    }
}

#[async_trait]
impl HasPermission<SqlitePool> for Member {
    async fn has(&self, _perm: &str, _pool: &Option<&SqlitePool>) -> bool {
        true
    }
}

impl Member {
    pub async fn get_member(id: i64, pool: &SqlitePool) -> Option<Self> {
        let sqlmember = sqlx::query_as::<_, SqlMember>("SELECT * FROM members WHERE id = $1")
            .bind(id)
            .fetch_one(pool)
            .await
            .ok()?;

        Some(sqlmember.into_member())
    }

    pub async fn create_member_tables(pool: &SqlitePool) {

    }
}
