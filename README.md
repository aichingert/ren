# Notes
I wanted to create a website to use for me to take notes in school

## Features
* Store notes with a database
* Represent notes on the home page with a littel text as description
* Notes should be written in asciidoctor so the website can show how it should look like on the website while writing

## Milestones
Phase | Milestone / Result | Date |
|--| -----| | --|
| Create Style | Finish the designing and scss for the website | 20-01-2023 |
| Create Backend | Finish database integration for storing notes | 30-02-2023 |
| Create Asciidoc visualizer | Finish to show the written asciidoc on the website |

## Setup
If you want to start the website you need to install 

`cargo install trunk`, `cargo install wasm-bindgen-cli` and `rustup target add wasm32-unknown-unknown`

after you installed these three, you can start it with `trunk serve`
