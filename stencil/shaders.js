var fragmentShaderG = require('fragmentShaderG.c');
var fragmentShaderH = require('fragmentShaderH.c');
var fragmentShaderM = require('fragmentShaderM.c');
var fragmentShaderS = require('fragmentShaderS.c');
var fragmentShaderT = require('fragmentShaderT.c');
var vertexShaderG = require('vertexShaderG.c');
var vertexShaderH = require('vertexShaderH.c');
var vertexShaderM = require('vertexShaderM.c');
var vertexShaderS = require('vertexShaderS.c');
var vertexShaderT = require('vertexShaderT.c');

module.exports = function (type, name) {
    var shader;
    if(type == 'vertex'){
        shader = gl.createShader('gl.VERTEX_SHADER');
        switch(name){
            case 'G':
            gl.shaderSource(shader, vertexShaderG());
            break;
            case 'H':
            gl.shaderSource(shader, vertexShaderH());
            break;
            case 'M':
            gl.shaderSource(shader, vertexShaderM());
            break;
            case 'S':
            gl.shaderSource(shader, vertexShaderS());
            break;
            case 'T':
            gl.shaderSource(shader, vertexShaderT());
            break;
        }
    }
    if(type == 'fragment'){
        shader = gl.createShader('gl.FRAGMENT_SHADER');
        switch(name){
            case 'G':
            gl.shaderSource(shader, fragmentShaderG());
            break;
            case 'H':
            gl.shaderSource(shader, fragmentShaderH());
            break;
            case 'M':
            gl.shaderSource(shader, fragmentShaderM());
            break;
            case 'S':
            gl.shaderSource(shader, fragmentShaderS());
            break;
            case 'T':
            gl.shaderSource(shader, fragmentShaderT());
            break;
        }
    }
    gl.compileShader(shader);
    return shader
}
