/**
 * This file contains all mouse and keyboard controls
 */


// constant rotation around y axis at m_flipfreq * 10
var m_animateSquare = 0;
var m_animateOn = false;

// zoom range & init
var m_maxZ = -0.1;
var m_minZ = -20.0;
var m_initZ = -3.8;
var m_stepZ = 5.0;

var initBrightness = 0; 
var initContrast = 100; 
var initGamma = 50; 

var minBrightness = -100;
var maxBrightness = 100;
var minContrast = 0;
var maxContrast = 200;
var minGamma = 0;
var maxGamma = 100;

var settings = {
    zoom: m_initZ,
    flipH :  false,
    flipV :  false,
	invert :  false,
	brightness : initBrightness,
	contrast : initContrast,
	gamma : initGamma,
	rotation : 0.0
}

// Position & Rotation
var transX = 0.0;
var transY = 0.0;

// Mouse 
var mouseDownLeft = false;  // used for pan (hand drag)
var mouseDownRight = false; // used for brightness/contrast adjustment
var lastMouseX = null;
var lastMouseY = null;

// Toggle the debug animation
function toggleAnimate(){
	m_animateOn = !m_animateOn;
}

function rotLeft90(){
	settings.rotation = pi / 2;
}
function rotRight90(){
	settings.rotation = 3 *( pi / 2 );
}

// Reset all control vars
function reset() {
	transY = 0.0;
	transX = 0.0;
	
	settings.rotation = 0.0;
	settings.zoom = m_initZ;
	settings.flipH = false;
	settings.flipV = false;
	m_animateSquare = 0;
	m_animateEven = 0;
	m_animateOdd = 0;
	m_animateOn = false;
	
	settings.invert = false; 
	settings.brightness = initBrightness; 
 	settings.contrast = initContrast; 
 	settings.gammra = initGamma; 
 	updateBCG(settings.brightness, settings.contrast, settings.gamma);
}

// Flip image over its X axis
function flipH() {
	settings.flipH = !settings.flipH;
}

// Flip image over its Y axis
function flipV() {
	settings.flipV = !settings.flipV;
}

// Flip over X axis relative to view
function flipX() {
	/// TODO:
}

// Flip over X axis relative to view
function flipY() {
	/// TODO: 
}

function handleMouseDown(e) {

	if (!e.which && e.button) {
		if (e.button & 1) e.which = 1      // Left
    	else if (e.button & 4) e.which = 2 // Middle
		else if (e.button & 2) e.which = 3 // Right
	}
	if(e.which == 1){
    	mouseDownLeft = true;
    }
    if(e.which == 3){
    	mouseDownRight = true;
    }
    lastMouseX = e.clientX;
    lastMouseY = e.clientY;
}

function handleMouseUp(e) {
		if (!e.which && e.button) {
		if (e.button & 1) e.which = 1      // Left
    	else if (e.button & 4) e.which = 2 // Middle
		else if (e.button & 2) e.which = 3 // Right
	}
	if(e.which == 1){
    	mouseDownLeft = false;
    }
    if(e.which == 3){
    	mouseDownRight = false;
    }
}

function setContrast ( val ){
	settings.contrast = val;
	if(settings.contrast < minContrast){
	 	settings.contrast = minContrast;
	 } 
	 if(settings.contrast > maxContrast){
	 	settings.contrast = maxContrast;
	 }
}

function setBrightness( val ){
	settings.brightness = val;
	if(settings.brightness < minBrightness){
		settings.brightness = minBrightness;
	}
	if(settings.brightness > maxBrightness){
		settings.brightness = maxBrightness;
	}
}

function handleMouseMove(event) {
	var newX = event.clientX;
    var newY = event.clientY;

    var deltaX = newX - lastMouseX;
    var deltaY = newY - lastMouseY;
    
    if ( !mouseDownLeft ) {
		if(mouseDownRight){
			var dy = deltaY / 100.0;
			var dx = deltaX / 100.0;
			// Adjust BCG
			setBrightness (settings.brightness - dy);
	 		setContrast (settings.contrast - dx);
			
			updateBCG( settings.brightness, settings.contrast, settings.gamma );
		}
		return;
    }



    if(isCommandKeyDown()){

	    if(deltaX > 0){
	    	for(var i = 0; i < deltaX; i++){
	    		panRight();
	    	}
	    }else{
	    	for(var i = 0; i < -deltaX; i++){
	    		panLeft();
	    	}
	    }

	    if(deltaY > 0){
	    	for(var i = 0; i < deltaY; i++){

	    		panDown();
	    	}
	    }else{
	    	for(var i = 0; i < -deltaY; i++){
	    		panUp();
	    	}
	    }
    }

    lastMouseX = newX
    lastMouseY = newY;
}

var currentlyPressedKeys = {};
function mouseWheelHandler(e){
	// cross-browser wheel delta
	 if(isCommandKeyDown()){
		var e = window.event || e; // old IE support
		var delta = Math.max(-1, Math.min(1, (e.wheelDelta || -e.detail)));

		settings.zoom += delta / m_stepZ;
		if(settings.zoom > m_maxZ){
			settings.zoom = m_maxZ;
		}
	}
}

/// TODO: this is platform and browser specific.
function commandKeyL(){
	if(window.navigator.platform == "MacIntel"){
		return 91; // command key in chrome & safari on mac. doesn't work in firefox
	} 
	return 18;
}

function commandKeyR(){
	if(window.navigator.platform == "MacIntel"){
		return 93; // command key in chrome & safari on mac. doesn't work in firefox
	} 
	return 18;
}

function isCommandKeyDown(){
	return currentlyPressedKeys[commandKeyL()] || currentlyPressedKeys[commandKeyR()];
}

/// TODO: event seems to only be handled incorrectly for multiple keys up.
/// Example, holding command while rotation is applied then releasing rotate causes indefinite spin
function handleKeyUp(event) {
	currentlyPressedKeys[event.keyCode] = false;
}

function handleKeyDown(event){
	if(event.keyCode == 65) { // a key
		toggleAnimate();
	}
	if(event.keyCode == 86) { // v key
		flipV();
	}
	if(event.keyCode == 72) { // h key
		flipH();
	}
	
  	currentlyPressedKeys[event.keyCode] = true;
}

function zoomIn(){
    if (settings.zoom >=  m_maxZ) {
    	settings.zoom = m_maxZ;
    }else{
    	settings.zoom += 0.001;
    }
}

function zoomOut(){
    if (settings.zoom <=  m_minZ) {
        settings.zoom = m_minZ;
    }else{
    	settings.zoom -= 0.001;
	}
}

var panBase = 0.002;

function panUp()   {
	transY += -settings.zoom * panBase;
}
function panDown() {
	transY -= -settings.zoom * panBase;
}
function panLeft() {
	transX -= -settings.zoom * panBase;
}
function panRight(){
	transX += -settings.zoom * panBase;
}

function rotateLeft() { settings.rotation -= 0.03; }
function rotateRight() { settings.rotation += 0.03; }

function invert(){
	settings.invert = !settings.invert;
}

function handleKeys() {

	if(currentlyPressedKeys[73]) { // i key
		invert();	
	}
	if(currentlyPressedKeys[82]) { // r key
		reset();
	}
	if(currentlyPressedKeys[33]) { // Page Up
   		zoomIn();
	}
   	if(currentlyPressedKeys[34]){ // Page Down
    	zoomOut()
	}
	if(currentlyPressedKeys[37]){ // LEft
		panLeft();
	}
	if(currentlyPressedKeys[38]){ // UP
		panUp();
	}
	if(currentlyPressedKeys[39]){ // RIGHT
		panRight();
	}
	if(currentlyPressedKeys[40]){ // DOWN
		panDown();
	}
	if(currentlyPressedKeys[35]){ // END
		rotateRight();
	}
	if(currentlyPressedKeys[36]){ // HOME
		rotateLeft();
	}
}
