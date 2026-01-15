class Topic {
  constructor(text, level, href) {
    this.text = text;
    this.level = level;
    this.href = href;
    this.children = [];
    this.expanded = false;
  }
}

const rootTopic = new Topic("", 0, "index.html");
const topics = [rootTopic];
console.log(rootTopic);

const tocRe = new RegExp("^toc([1-6])$");
for (const a of document.getElementsByTagName("a")) {
  const matches = tocRe.exec(a.className);
  //console.log(matches[1] + ": " + a.innerText + " -> " + a.href);
  a.onclick = function() { 
      window.parent.postMessage({
        "type": "topicNav",
        "topic": a.href
    }, "*");
    return false;
  };
}
