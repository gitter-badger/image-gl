
// Variables used for animating flips & rotation
var m_flipfreq = 4000.0;
var m_rotx = 0;
var m_roty = 0;
var m_rotz = 0;
var lastTime = 0;


// Animate is called in tick() 
function animate() {
    var timeNow = new Date().getTime();
    if (lastTime != 0) {
		var elapsed = timeNow - lastTime;
		m_animateSquare += (75 * elapsed) / (m_flipfreq * 10) ;

		if(settings.flipH){
			// increase from 0 to pi
			m_rotx += (75 *elapsed) / m_flipfreq;
			if(m_rotx > pi){
				m_rotx = pi;
			}
		}else{
			// decrease from pi to 0
			m_rotx -= (75 * elapsed) / m_flipfreq;
			if(m_rotx < 0.0){
				m_rotx = 0.0
			}
		}
		if(settings.flipV){
			// increase from 0 to pi
			m_roty += (75 * elapsed) / m_flipfreq;
			if(m_roty > pi){
				m_roty = pi;
			}
		}else{
			// decrease from pi to 0
			m_roty -= (75 * elapsed) / m_flipfreq;
			if(m_roty < 0.0){
				m_roty = 0.0;
			}
		}

		if(m_rotz < settings.rotation){
			m_rotz += (75 * elapsed) / m_flipfreq;
			if(m_rotz > settings.rotation){
				// rotated too far
				m_rotz = settings.rotation;
			}
		}
		if(m_rotz > settings.rotation){
			m_rotz -= (75 * elapsed) / m_flipfreq;
			if(m_rotz > settings.rotation){
				// rotated too far
				m_rotz = settings.rotation;
			}
		}
    }
	lastTime = timeNow;
}