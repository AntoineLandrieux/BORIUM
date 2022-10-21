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
