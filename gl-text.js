// delphic.me.uk/webgltext.html

function getPowerOfTwo(value, pow) {
    var pow = pow || 1;
    while(pow<value) {
        pow *= 2;
    }
    return pow;
}

var canvas = document.getElementById('textureCanvas');
var ctx = canvas.getContext('2d');

var textToWrite  = "Test text";

var textSize = 12;
ctx.font = textSize+"px monospace"; 	// Set the font of the text before measuring the width!
var canvas.width = getPowerOfTwo(ctx.measureText(textToWrite).width);
var canvas.height = getPowerOfTwo(2*textSize);

ctx.fillStyle = "#333333"; 	// This determines the text colour, it can take a hex value or rgba value (e.g. rgba(255,0,0,0.5))
ctx.textAlign = "center";	// This determines the alignment of text, e.g. left, center, right
ctx.textBaseline = "middle";	// This determines the baseline of the text, e.g. top, middle, bottom
ctx.font = "12px monospace";	// This determines the size of the text and the font family used

ctx.fillText(m_text, canvas.width/2, canvas.height/2);

function createMultilineText(ctx, textToWrite, maxWidth, text) {
    textToWrite = textToWrite.replace("\n"," ");
    var currentText = textToWrite;
    var futureText;
    var subWidth = 0;
    var maxLineWidth = 0;

    var wordArray = textToWrite.split(" ");
    var wordsInCurrent, wordArrayLength;
    wordsInCurrent = wordArrayLength = wordArray.length;

    // Reduce currentText until it is less than maxWidth or is a single word
    // futureText var keeps track of text not yet written to a text line
    while (measureText(ctx, currentText) > maxWidth && wordsInCurrent > 1) {
        wordsInCurrent--;
        var linebreak = false;

        currentText = futureText = "";
        for(var i = 0; i < wordArrayLength; i++) {
            if (i < wordsInCurrent) {
                currentText += wordArray[i];
                if (i+1 < wordsInCurrent) { currentText += " "; }
            }
            else {
                futureText += wordArray[i];
                if(i+1 < wordArrayLength) { futureText += " "; }
            }
        }
    }
    text.push(currentText); // Write this line of text to the array
    maxLineWidth = measureText(ctx, currentText);

    // If there is any text left to be written call the function again
    if(futureText) {
        subWidth = createMultilineText(ctx, futureText, maxWidth, text);
        if (subWidth > maxLineWidth) {
            maxLineWidth = subWidth;
        }
    }

    // Return the maximum line width
    return maxLineWidth;
}