import Image from 'next/image'
import { Inter } from 'next/font/google'
import React from "react";

const inter = Inter({ subsets: ['latin'] })

export default function MyHome() {
  const [likes, setLikes] = React.useState(0);

  function handleClick() {
    setLikes(likes + 1);
  }

  return (
      <button onClick={handleClick}>Likes ({likes})</button>
  );
}