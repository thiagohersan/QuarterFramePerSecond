var video = document.createElement("video");
video.setAttribute("style", "display:none;");
video.setAttribute("id", "videoOutput");
video.setAttribute("width", "800");
video.setAttribute("height", "480");
video.setAttribute("autoplay", "true");

if(document.body!=null) document.body.appendChild(video);
 
function init() {
    if (navigator.webkitGetUserMedia) {
        navigator.webkitGetUserMedia({video:true}, gotStream, noStream);
 
        function gotStream(stream) {
            video.src = webkitURL.createObjectURL(stream);
            video.onerror = function () {
                stream.stop();
                streamError();
            };
        }
        
        function noStream() {
            alert("No camera available.");
        }
 
        function streamError() {
            alert("Camera error.");
        }
    }
}
