const input = document.getElementById("query"); 

fetch("http://localhost:8080/api/lucky")
	.then((response) => response.json())
	.then((data) => document.getElementById("req").innerHTML = data);

input.addEventListener("input", function() { 
	document.getElementById("out").innerHTML = input.value;
});
