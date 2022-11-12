const nav = document.querySelector("nav");
const btn = document.querySelector("button");
const logo = document.querySelector("img");

window.addEventListener('load', function (event)
{
    if (event.isTrusted == true) {
        document.getElementById("load").style.visibility = "hidden";
    }
});

btn.addEventListener('click', function (f) {
    nav.classList.toggle("enable");
    nav.classList.toggle("disable");
    btn.classList.toggle("enable");
    btn.classList.toggle("disable");
});
