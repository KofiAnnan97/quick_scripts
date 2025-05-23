use serde_json::{Result, Value, Error};
use serde::{Deserialize, Serialize};
use std::f64;

// Structs
#[derive(Deserialize, Serialize, Debug)]
pub struct Game {
    pub title: String,
    pub id: u64,
    pub price: PriceOverview,
}

#[derive(Deserialize, Serialize, Debug)]
pub struct PriceOverview {
    pub currency: String,
    pub amount: String,
    #[serde(rename="baseAmount")]
    pub base_amount: String,
    #[serde(rename="finalAmount")]
    pub final_amount: String,
    #[serde(rename="isDiscounted")]
    pub is_discounted: bool,
    #[serde(rename="discountPercentage")]
    pub discount_percentage: i32,
    #[serde(rename="discountDifference")]
    discount_diff: String,
    pub symbol: String,
    #[serde(rename="isFree")]
    pub is_free: bool,
    pub discount: i32,
    #[serde(rename="isBonusStoreCreditIncluded")]
    is_bonus_credit_included: bool,
    #[serde(rename="bonusStoreCreditAmount")]
    bonus_credit_amount: String
}

pub async fn search_game_by_title(title: &str) -> Result<Vec<Game>> {
    let http_client = reqwest::Client::new();
    let media_type = "game";
    let limit :i32 = 30;
    let url = format!("https://embed.gog.com/games/ajax/filtered?mediaType={}&search={}&limit={}", media_type, title, limit);
    let resp = http_client.get(url)
        .send()
        .await
        .expect("Failed to get response")
        .text()
        .await
        .expect("Failed to get data");
    let body : Value = serde_json::from_str(&resp).expect("Could not convert to JSON");
    let products = serde_json::to_string(&body["products"]).unwrap();
    let games_list : Vec<Game> = serde_json::from_str::<Vec<Game>>(&products)?;
    Ok(games_list)
}

pub fn get_price_from_list(title:&str, games_list: Vec<Game>) -> Option<f64> {
    for game in games_list.iter(){
        if title == &game.title {
            let game_price : f64 = game.price.final_amount.parse::<f64>().unwrap();
            return Ok::<f64, Error>(game_price).ok();
        } 
    }
    None
}

pub async fn get_price(title: &str) -> Option<PriceOverview> {
    let http_client = reqwest::Client::new();
    let media_type = "game";
    let limit_num : i32 = 30;
    let url = format!("https://embed.gog.com/games/ajax/filtered?mediaType={}&search={}&limit={}", media_type, title, limit_num);
    let resp = http_client.get(url)
        .send()
        .await
        .expect("Failed to get response")
        .text()
        .await
        .expect("Failed to get data");
    let body: Value = serde_json::from_str(&resp).expect("Could not convert to JSON");
    if let Some(products) = body["products"].as_array() {
        for idx in 0..products.len(){
            let game_title = products[idx]["title"].to_string();
            if title.to_string() == game_title[1..game_title.len()-1].to_string(){
                let price = serde_json::to_string(&products[idx]["price"]).unwrap();
                let price_overview = serde_json::from_str::<PriceOverview>(&price).unwrap();
                return Ok::<PriceOverview, Error>(price_overview).ok();
            }
        }
    }
    None
}