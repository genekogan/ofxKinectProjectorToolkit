#version 120

uniform sampler2DRect texture;
//uniform vec2 texOffset;

const vec2 texOffset = vec2(1.0, 1.0);

void main(void) {
  // Grouping texcoord variables in order to make it work in the GMA 950. See post #13
  // in this thread:
  // http://www.idevgames.com/forums/thread-3467.html
  vec2 st = gl_FragCoord.st;
  vec2 tc0 = st + vec2(-texOffset.s, -texOffset.t);
  vec2 tc1 = st + vec2(         0.0, -texOffset.t);
  vec2 tc2 = st + vec2(+texOffset.s, -texOffset.t);
  vec2 tc3 = st + vec2(-texOffset.s,          0.0);
  vec2 tc4 = st + vec2(         0.0,          0.0);
  vec2 tc5 = st + vec2(+texOffset.s,          0.0);
  vec2 tc6 = st + vec2(-texOffset.s, +texOffset.t);
  vec2 tc7 = st + vec2(         0.0, +texOffset.t);
  vec2 tc8 = st + vec2(+texOffset.s, +texOffset.t);
  
  vec4 col0 = texture2DRect(texture, tc0);
  vec4 col1 = texture2DRect(texture, tc1);
  vec4 col2 = texture2DRect(texture, tc2);
  vec4 col3 = texture2DRect(texture, tc3);
  vec4 col4 = texture2DRect(texture, tc4);
  vec4 col5 = texture2DRect(texture, tc5);
  vec4 col6 = texture2DRect(texture, tc6);
  vec4 col7 = texture2DRect(texture, tc7);
  vec4 col8 = texture2DRect(texture, tc8);

  vec4 sum = 8.0 * col4 - (col0 + col1 + col2 + col3 + col5 + col6 + col7 + col8); 
  gl_FragColor = vec4(sum.rgb, 1.0) * col4;
}
