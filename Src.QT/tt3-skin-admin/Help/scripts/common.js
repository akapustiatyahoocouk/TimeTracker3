function getParameterByName(name, url = window.location.href) {
    name = name.replace(/[\[\]]/g, '\\$&');
    var regex = new RegExp('[?&]' + name + '(=([^&#]*)|&|#|$)'),
        results = regex.exec(url);
    if (!results) return null;
    if (!results[2]) return '';
    return decodeURIComponent(results[2].replace(/\+/g, ' '));
}

function resize_iframes() {
  let ih = window.innerHeight;
  //console.log(ih);
  const hh = document.querySelector('#home').clientHeight;
  const fh = document.querySelector('#footer').clientHeight;
  document.querySelector('#navigatorframe').height = ih - (hh + fh + 50); //164;
  document.querySelector('#contentframe').height = ih - (hh + fh + 50); //164;
}

window.addEventListener("resize", (event) => {
  resize_iframes();
});
resize_iframes();

//////////
//  Determine the pages to display in "toc" and "content" iframes...
const tocUrl = "toc.htm";
const contentUrl = "index.html";
console.log(tocUrl);
console.log(contentUrl);
//  ...and load them
document.querySelector('#navigatorframe').src = tocUrl;
document.querySelector('#contentframe').src = contentUrl ;

if (window.addEventListener) {
    window.addEventListener("message", onMessage, false);        
} 
else if (window.attachEvent) {
    window.attachEvent("onmessage", onMessage, false);
}

function onMessage(event) {
    //alert(event.data.topic);
    if (event.data.type == "topicNav") {
      document.querySelector('#contentframe').src = event.data.topic;
    }
}
