const nav = document.querySelector("nav");
const btn = document.querySelector("button");
const logo = document.querySelector("img");

btn.addEventListener('click', function (f) {
    nav.classList.toggle("enable");
    nav.classList.toggle("disable");
    btn.classList.toggle("enable");
    btn.classList.toggle("disable");
});

window.addEventListener('scroll', function (f) {
    if (window.scrollY > 200)
    {
        logo.style.transform = `translate(0, ${window.scrollY/2}px)`;
    }
    else {
        logo.style.transform = "translate(0, 0)";
    }
});

const suggestions = [
    {name: "Information", redirect: "whatisit"},
    {name: "Télécharger", redirect: "download"},
    {name: "Main Menu",   redirect: "main"},
    {name: "License",     redirect: "ref"}
];

const searchInput = document.getElementById("search");
const suggestion = document.getElementById("sugg");

searchInput.addEventListener('keyup', function (f) {
    let sug = '';
    input = searchInput.value;
    if (input != '') {
      const result = suggestions.filter(Item => Item.name.toLowerCase().includes(input.toLowerCase()));
      result.forEach(resultItem => sug += `<a href="#${resultItem.redirect}">${resultItem.name}</a>`);    
    }
    suggestion.innerHTML = sug;
});