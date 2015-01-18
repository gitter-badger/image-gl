
// Variables used for animating flips & rotation
var m_flipfreq = 4000.0;
var m_rotx = 0;
var m_roty = 0;
var m_rotz = 0;
var lastTime = 0;

var m_animateEven = 0;
var m_animateOdd = 0;

// Animate is called in tick() 
function animate() {
    var timeNow = new Date().getTime();
    if (lastTime != 0) {
		var elapsed = timeNow - lastTime;

		if(m_animateOn){
			m_animateSquare += (75 * elapsed) / (m_flipfreq * 50) ;
			m_animateEven += (75 * elapsed) / (m_flipfreq * 40) ;
			m_animateOdd += (75 * elapsed) / (m_flipfreq * 20) ;
		}else{
			if(m_animateSquare > 0){
				m_animateSquare -= (75 * elapsed) / (m_flipfreq * 5) ;
				if(m_animateSquare < 0){
					m_animateSquare = 0;
				}
			}
			if(m_animateEven > 0){
				m_animateEven -= (75 * elapsed) / (m_flipfreq * 4) ;
				if(m_animateEven < 0){
					m_animateEven = 0;
				}
			}
			if(m_animateOdd > 0){
				m_animateOdd -= (75 * elapsed) / (m_flipfreq * 2) ;
				if(m_animateOdd < 0){
					m_animateOdd = 0;
				}
			}
		}
		
        if(settings.zoom != zoomZ){
            if(settings.zoom > zoomZ){
                var step = (settings.zoom - zoomZ) / 16.0;
                zoomZ += step;
                if(zoomZ > m_maxZ){
                    zoomZ = m_maxZ;
                }
            }
            if(settings.zoom < zoomZ){
                var step = (settings.zoom - zoomZ) / 16.0;
                zoomZ += step;
                if(zoomZ < m_minZ){
                    zoomZ = m_minZ;
                }
            }
        }
		
		
	    if(settings.transX != transX){
            if(settings.transX > transX){
                var step = (settings.transX - transX) / 5.0;
                transX += step;

            }
            if(settings.transX < transX){
                var step = (settings.transX - transX) / 5.0;
                transX += step;
            }
        }

        if(settings.transY != transY){
            if(settings.transY > transY){
                var step = (settings.transY - transY) / 5.0;
                transY += step;

            }
            if(settings.transY < transY){
                var step = (settings.transY - transY) / 5.0;
                transY += step;
            }
        }

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