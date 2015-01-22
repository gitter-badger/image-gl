/**
 * This file contains all mouse and keyboard controls
 */

// constant rotation around y axis at m_flipfreq * 10
var m_animateSquare = 0;
var m_animateOn = false;

// zoom range & init
var m_maxZ = -0.1;
var m_minZ = -20.0;
var m_initZ = -8.0;
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

var m_brightness = initBrightness;
var m_contrast = initContrast;
var m_gamma = initGamma;

var settings = {
    zoom: m_initZ,
    flipH :  false,
    flipV :  false,
	invert :  false,
	brightness : initBrightness,
	contrast : initContrast,
	gamma : initGamma,
	rotation : 0.0,
	transX : 0.0,
	transY : 0.0
}

// Position & Rotation
var transX = 0.0;
var transY = 0.0;
var zoomZ = m_initZ;

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

function resetSettings() {
	m_animateSquare = 0;
	m_animateEven = 0;
	m_animateOdd = 0;
	m_animateOn = false;
	
	settings.invert = false; 
	settings.brightness = initBrightness; 
 	settings.contrast = initContrast; 
 	settings.gamma = initGamma;
 	
 	updateBCG(settings.brightness, settings.contrast, settings.gamma);
 	updateInvert(settings.invert);
}

function toggleOsteotomy() {
    alert('Osteotomy not available');
}

function resetColor() {
    settings.brightness = initBrightness;
    settings.contrast = initContrast;
    settings.gamma = initGamma;
}

// Reset all control vars
function reset() {
	
	resetSettings(); 
	
	transY = 0.0;
	transX = 0.0;
	
	settings.rotation = 0.0;
	settings.zoom = m_initZ;
	settings.flipH = false;
	settings.flipV = false;
	settings.transX = 0.0;
	settings.transY = 0.0;
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

function setGamma( val ){
	settings.gamma = val;
	if(settings.gamma < minGamma){
		settings.gamma = minGamma;
	}
	if(settings.gamma > maxGamma){
		settings.gamma = maxGamma;
	}
}

function handleMouseMove(event) {
	var newX = event.clientX;
    var newY = event.clientY;

    var deltaX = newX - lastMouseX;
    var deltaY = newY - lastMouseY;
    
    if ( !mouseDownLeft ) {
		if(mouseDownRight){
			
			var dy = deltaY / 30.0;
			var dx = deltaX / 30.0;
						
			// Adjust BCG
			if(!isCommandKeyDown()){
				setBrightness (settings.brightness - dy);
	 			setContrast (settings.contrast - dx);
			}else{
				setGamma(settings.gamma - dx);
			}
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
	var e = window.event || e; // old IE support
	var delta = Math.max(-1, Math.min(1, (e.wheelDelta || -e.detail)));

	// cross-browser wheel delta
	 if(isCommandKeyDown()){
		settings.zoom += delta / m_stepZ;
		if(settings.zoom > m_maxZ){
			settings.zoom = m_maxZ;
		}
	}
	if(isCtrlKeyDown()){
		var amt = 0;
		if( delta > 0 ){
			amt = 1;
		}
		if( delta < 0 ){
			amt = -1;
		}
		amt = amt / ( 90 );
		settings.rotation += ( amt ) ;
	}	
}

function ctrlKey(){
	return 17;
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

function isCtrlKeyDown() {
	return currentlyPressedKeys[ctrlKey()];
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
	if(event.keyCode == 73) { // i key
		invert();
	}
	
  	currentlyPressedKeys[event.keyCode] = true;
}

function zoomIn(){
    if (settings.zoom >=  m_maxZ) {
    	settings.zoom = m_maxZ;
    }else{
    	settings.zoom += 0.01;
    }
}

function zoomOut(){
    if (settings.zoom <=  m_minZ) {
        settings.zoom = m_minZ;
    }else{
    	settings.zoom -= 0.01;
	}
}

var panBase = 0.001;

var rotMatrix = [ 1, 0, 
				  0, 1]

function updateRotMatrix () {
	var angle = settings.rotation;
	rotMatrix = [ 
		cos(angle), asin(angle),
		-sin(angle), cos(angle)
	];
}

function panUp()   {
	var y = -settings.zoom * panBase;
	var x = 0.0;
	settings.transX += x * Math.cos(settings.rotation) + y * Math.sin(settings.rotation);
	settings.transY += y * Math.cos(settings.rotation) - x * Math.sin(settings.rotation);	
}
function panDown() {
	var y = settings.zoom * panBase;
	var x = 0.0;
	settings.transX += x * Math.cos(settings.rotation) + y * Math.sin(settings.rotation);
	settings.transY += y * Math.cos(settings.rotation) - x * Math.sin(settings.rotation);	
}
function panLeft() {
	var x = settings.zoom * panBase;
	var y = 0.0;
	settings.transX += x * Math.cos(settings.rotation) + y * Math.sin(settings.rotation);
	settings.transY += y * Math.cos(settings.rotation) - x * Math.sin(settings.rotation);	
}
function panRight() {
	var x = -settings.zoom * panBase;
	var y = 0.0;;
	settings.transX += x * Math.cos(settings.rotation) + y * Math.sin(settings.rotation);
	settings.transY += y * Math.cos(settings.rotation) - x * Math.sin(settings.rotation);
}

function rotateLeft() { settings.rotation -= pi / 190; }
function rotateRight() { settings.rotation += pi / 180; }

function invert() {
	settings.invert = !settings.invert;
	updateInvert(settings.invert);
}

function handleKeys() {

	if(currentlyPressedKeys[73]) { // i key
		invert();	
	}
	if(currentlyPressedKeys[82]) { // r key
		if(isCtrlKeyDown()){
			resetSettings();
		}else{
			reset();
		}
	}

    if(currentlyPressedKeys[79]){ // O key
        toggleOsteotomy();
    }

    if(currentlyPressedKeys[67]){ // C key
        resetColor();
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
