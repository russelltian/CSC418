

/**
 * Note: Do not include any external libraries.
 * You may only use the drawing functions provided to complete this assignment.
 */

/**    HELPER FUNCTIONS     **/
/** Do not modify this code **/

/**
 * Draws a line segment on the screen connecting (x1, y1) and (x2, y2). This is the only function
 * you may call to draw.
 * @param  {DOMString} ctx - Canvas context to draw to. Simply pass in the given ctx variable
 * @param  {number} x1 - X coordinate of the start point
 * @param  {number} y1 - Y coordinate of the start point
 * @param  {number} x2 - X coordinate of the end point
 * @param  {number} y2 - Y coordinate of the end point
 * @param  {String} style - A string giving the colour of the line. Default is black.
 * @return {undefined}
 */
function drawLineSegment(ctx, x1, y1, x2, y2, style="black")
{
  ctx.beginPath();
  ctx.moveTo(x1,y1);
  ctx.lineTo(x2,y2);
  ctx.strokeStyle = style;
  ctx.stroke();
}

/**
 * Takes a point defined as an array of numbers [x, y] and treats it as a homogeneous coordinate [x, y, 1]
 * The transformation T is a 2x3 matrix defined as an array of arrays
 * T = [[T11, T12, T13],
 *      [T21, T22, T23]]
 * The result is a new transformed point [x', y'] = T * [p[0], p[1], 1]
 * @param  {array} p - Point to transform
 * @param  {array} T - Transformation to apply
 * @return {array} New transformed point
 */
function transformPoint(p,T)
{
  var q = new Array();

  for(var i = 0; i<T.length; i++)
  {
    q[i] = 0;
    for(var j = 0; j<p.length; j++)
      q[i] += T[i][j] * p[j];
	// Translation
	q[i] += T[i][j];
  }

  return q;
}

/**
 * Given two transformations T1 and T2, returns a new transformation equivalent to applying
 * T1 followed by T2.
 * @param  {array} T1 - The first transform to apply
 * @param  {array} T2 - The second transfrom to apply
 * @return {array} A new transformation
 */
function composeTransforms(T1, T2) {
  var T_new = [
    [0, 0, 0],
    [0, 0, 0]
  ];

  for (var i = 0; i < 2; i++) {
    for (var j = 0; j < 3; j++) {
      for(var k = 0; k < 3; k++) {
        T_new[i][j] += T2[i][k] * (k === 2 ? (j === 2 ? 1 : 0) : T1[k][j]);
      }
    }
  }

  return T_new;
}
/** END OF HELPER FUNCTIONS **/


/**
 * This function should take a list of points and draw a closed polygon connecting the points.
 * Each point is an array containing two numbers [x, y].
 * @param  {DOMString} ctx - Canvas context to draw to. Simply pass in the given ctx variable
 * @param  {array} poly - Array of points corresponding to the vertices of the polygon [p1, p2, p3, ..., pn]
 * @return {undefined}
 */
function drawPolygon(ctx, poly)
{
  /** Fill in your answer here **/
  // Hint: call drawLineSegment to help you
    // my code, first connect each ajacent point from start to end - 1, finally ,connect end to start
    // to connect the line, use drawLineSegment
    for (var i = 0; i < poly.length - 1 ; i++){
        drawLineSegment(ctx, poly[i][0], poly[i][1], poly[i+1][0], poly[i+1][1], "black");
    }
    drawLineSegment(ctx,poly[poly.length -1][0],poly[poly.length - 1][1],poly[0][0],poly[0][1],"black");
}

/**
 * Given an array of points defining a polygon, return a new array of points for a polygon transformed by
 * the transformation T.
 * @param  {array} poly - Array of points corresponding to the vertices of the polygon [p1, p2, p3, ..., pn]
 * @param  {array} T - T transformation to apply. Same definition as in transformPoint()
 * @return {array} Array of vertex locations for the transformed polygon
 */
function transformPolygon(poly, T)
{
  /** Fill in your answer here **/
  // Hint: call transformPoint to help you
   // var newpoly = new Array();
    //transfer each point by the transfrom matrix
    //pointwiselly transform each point
   for(var i = 0; i < poly.length;i++){
       poly[i] = transformPoint(poly[i],T);
   }

   //var newpoly =  transformPoint[poly,T];

   return poly;
}

/**
 * Draws a circle of radius r around point [cx, cy].
 * @param  {DOMString} ctx - Canvas context to draw to. Simply pass in the given ctx variable
 * @param  {number} cx - X coordinate of the circle center
 * @param  {number} cy - Y coordinate of the circle center
 * @param  {number} r - Radius of the circle
 * @return {undefined}
 */
function drawCircle(ctx, cx, cy, r)
{
  /** Fill in your answer here **/
  // Hint: call drawLineSegment to help you
    // I got this way from HTML5 online tutorial
    ctx.beginPath();
    ctx.arc(cx,cy,r,0,2*Math.PI);
    ctx.stroke();
}

/**
 * Returns a transformation matrix T that defines a translation in homogeneous coordinates
 * by the vector offset [x, y].
 * That is, if applied to a point p = [p1, p2, 1] then it will be
 * translated to p' = [p1 + x, p2 + y, 1]
 * @param  {array} offset - The offset to translate by
 * @return {array} A 2x3 transformation matrix
 */
function translateByOffset(offset)
{
  var T = [
    [1, 0, 0],
    [0, 1, 0]
  ];

  /** Fill in your answer here **/
    // as formula from the course
    //[1,0,dx]
    //[0,1,dy]
    T[0][2] = offset[0];
    T[1][2] = offset[1];
    return T;
}

/**
 * Returns a transformation matrix T that defines
 * a rotation by angle radians around a point joint.
 * @param  {number} angle - Angle in radians
 * @param  {array} joint - point [x, y] to rotate around
 * @return {array} T - 2x3 transformation matrix defining the rotation
 */
function rotationAboutPoint(angle, joint)
{
  var T = [
    [1, 0, 0],
    [0, 1, 0]
  ];

  /** Fill in your answer here **/
  // Hint: Use Math.cos() and Math.sin()

//first transform to the fixed point, then rotate, then transfer it back
// to compose the transform , we use composeTransform function
// M is defined by negative offset, and T is defined by rotation function from lecture s
 var M = [
    [1,0,-1*joint[0]],
    [0,1,-1*joint[1]]
    ];
    
    T = [
     [Math.cos(angle), -1*Math.sin(angle),0],
     [Math.sin(angle),Math.cos(angle),0]
    ];
    T = composeTransforms(M,T);
    //after the rotation, we need to transform it back by the positive offest
    M[0][2] = joint[0];
    M[1][2] = joint[1];
    
    T = composeTransforms(T,M);
    
  return T;
}


function drawPenguin(ctx)
{
  /** GET UI SLIDER VARIABLES **/
  /** You do not need to change this code **/
  var deg_to_rad = Math.PI / 180.0;
  var torso_x_offset = parseFloat(document.getElementById("slider_torso_x").value);
  var torso_y_offset = parseFloat(document.getElementById("slider_torso_y").value);
  var arm_angle = parseFloat(document.getElementById("slider_arm_angle").value) * deg_to_rad;
  var head_angle = parseFloat(document.getElementById("slider_head_angle").value) * deg_to_rad;
  var mouth_gap = parseFloat(document.getElementById("slider_mouth_gap").value);
  var hip_angles = [
    parseFloat(document.getElementById("slider_hip_angle0").value) * deg_to_rad,
    parseFloat(document.getElementById("slider_hip_angle1").value)  * deg_to_rad
  ];
  var ankle_angles = [
    parseFloat(document.getElementById("slider_ankle_angle0").value)  * deg_to_rad,
	parseFloat(document.getElementById("slider_ankle_angle1").value)  * deg_to_rad
  ];
  /** END OF GET UI SLIDER VARIABLES **/


  /** POLYGON DEFINITIONS **/
  /**
   * The polygons defining each body region are provided *relative to the origin*
   * It is your responsibility to transform and draw these polygons to construct a hierarchical model
   * that can be manipulated by the sliders.
   *
   * You may not change these values manually.
   */
  //
  var torso_poly = [[-56, -196], [-129, 99], [-46, 188], [36, 196], [129, 116], [69, -71], [36, -196]];
  var arm_poly = [[-38, -78], [-15, 78], [22, 78], [39, -74]];

  var head_poly = [[-19, -50], [-49, -29], [-62, 50], [62, 48], [46, -27]];
  var upper_beak_poly = [[41, -13], [-44, 2], [-41, 13], [43, 13]];
  var lower_beak_poly = [[-40, -3], [-42, 3], [41, 3], [41, -3]];

  var leg_poly = [[-15, -53], [-20, 52], [ 20,  52], [ 12, -53]];
  var foot_poly = [[54, -11], [58, 11], [-48,  11], [-58, -11]];

  // These will be drawn as circles
  var eye = [0, 0];
  var iris = [0, 0];

  // Radii of circles
  var eye_r = 10;
  var iris_r = 3;
  var arm_joint_r = 10;
  var head_joint_r = 5;
  var hip_joint_r = 5;
  var ankle_joint_r = 5;

  // Joint positions
  var hip_joints = [[0, 0], [0, 0]];
  var ankle_joints = [[0, 0], [0, 0]];
  var arm_joint = [0, 0];
  var head_joint = [0, 0];
  /** END OF POLYGON DEFINITIONS **/


  /*********************************/
  /** YOUR DRAWING CODE GOES HERE **/
  /*********************************/

  /** The torso and head are given as examples **/
  // Define the center of the torso in screen space
  // and a transformation to translate the torso to its origin
  var torso_origin = [360, 360];
  var torso_T = translateByOffset([torso_origin[0] + torso_x_offset, torso_origin[1] + torso_y_offset]);

  torso_poly = transformPolygon(torso_poly, torso_T);
  drawPolygon(ctx, torso_poly);

  // Define the transformation for the head as a rotation and then a translation
  var head_offset = [-10, -220]; // The location of the head relative to the body
  var head_joint_offset = [0, 35]; // The location of the head joint relative to the head center
  var head_T = composeTransforms(
    rotationAboutPoint(head_angle, head_joint_offset),
    translateByOffset(head_offset)
  );

  // Transform and draw the head in a hierarchical fashion
  // That is, if the body moves, then the head will move with it.
  head_poly = transformPolygon(head_poly, head_T);
  head_poly = transformPolygon(head_poly, torso_T);
  drawPolygon(ctx, head_poly);
    
  // Now draw the head joint
  head_joint = transformPoint(head_joint_offset, head_T);
  head_joint = transformPoint(head_joint, torso_T);
  drawCircle(ctx, head_joint[0], head_joint[1], head_joint_r);

  /* Draw the rest of the penguin below. */
    
  //eye and iris, they actually transform with the head, so the kinematic tree is head->body
  
    var eye_offset = [-20,-10]; // eye offset to the center of the head
    eye = transformPoint(eye_offset,head_T); //to the head coordinate
    eye = transformPoint(eye,torso_T); // to the world coordinate
    drawCircle(ctx, eye[0], eye[1], eye_r);
    iris = transformPoint(eye_offset,head_T); // same as eyes
    iris = transformPoint(iris,torso_T);
    drawCircle(ctx, iris[0],iris[1],iris_r);
    
    //mouth (upper and lower beak)
    var upper_beak_offset = [-90,0];
    var lower_beak_offset = [-90,30];
    //initialize upper and lower T
    //frist translate by offset, then rotate with head , then torso
    // beak->head->torso
    //the vertical offset is the beak offset plus the offset of mouth_gap
    var upper_beak_T = translateByOffset([upper_beak_offset[0],upper_beak_offset[1] - mouth_gap]);
    
    var lower_beak_T = translateByOffset([lower_beak_offset[0],lower_beak_offset[1] + mouth_gap]);
     
 
    //draw upper beak
    //beak->head->torso
    upper_beak_poly = transformPolygon(upper_beak_poly,upper_beak_T);
    upper_beak_poly = transformPolygon(upper_beak_poly,head_T);
    upper_beak_poly = transformPolygon(upper_beak_poly,torso_T);
    drawPolygon(ctx,upper_beak_poly);
    //draw lower beak
    lower_beak_poly = transformPolygon(lower_beak_poly,lower_beak_T);
    lower_beak_poly = transformPolygon(lower_beak_poly,head_T);
    lower_beak_poly = transformPolygon(lower_beak_poly,torso_T);
    drawPolygon(ctx,lower_beak_poly);
    
    //now move to arm, arm is a leaf of the body
    //first define the arm and its joint offset
    var arm_offset = [20,-10];
    var arm_joint_offset = [-10,-50];
    //define the arm transformation
    //arm -> body
    //rotate around the joint, and plus the offset of the body
    var arm_T = composeTransforms(
        rotationAboutPoint(arm_angle,arm_joint_offset),
        translateByOffset(arm_offset)
    );
    arm_poly = transformPolygon(arm_poly,arm_T);//body coordinate
    
    arm_poly = transformPolygon(arm_poly,torso_T);// world coordinate
    drawPolygon(ctx,arm_poly);
    //draw the arm joint
    arm_joint = transformPoint(arm_joint_offset,arm_T); // same as the arm
    arm_joint = transformPoint(arm_joint,torso_T);
    drawCircle(ctx,arm_joint[0],arm_joint[1],arm_joint_r);
    
    
    // draw the leg part, start with the leg
    // first the offset of leg0, its joint, and the transform
    var leg0_offset = [70,200];
    var leg0_joint_offset = [-5,-40];
    //first need to  rotate, then bring it to body coordinate
    var leg_T0 = composeTransforms(
            rotationAboutPoint(hip_angles[0], leg0_joint_offset),
            translateByOffset(leg0_offset)
    );
    var leg_poly2 = [];
    for(var i = 0; i < leg_poly.length;i++){
        leg_poly2[i] = leg_poly[i]; //a copy of origina leg shape in order to draw the second leg
    }
    //draw the first
    //leg->body then ->world
    leg_poly = transformPolygon(leg_poly,leg_T0);
    leg_poly = transformPolygon(leg_poly,torso_T);
    drawPolygon(ctx, leg_poly);
    hip_joints[0] = transformPoint(leg0_joint_offset,leg_T0);
    hip_joints[0] = transformPoint(hip_joints[0],torso_T);
    drawCircle(ctx,hip_joints[0][0],hip_joints[0][1],hip_joint_r);
    
    //define the second transform and draw the second leg
    // the same as the first leg, except the location of the joint, and the offset
    var leg1_offset = [-70,180];
    var leg1_joint_offset = [-5,-40];
    var leg_T1 = composeTransforms(
        rotationAboutPoint(hip_angles[1], leg1_joint_offset),
        translateByOffset(leg1_offset)
        );
    //draw the second
   
    leg_poly2 = transformPolygon(leg_poly2,leg_T1);
    leg_poly2 = transformPolygon(leg_poly2,torso_T);
    drawPolygon(ctx, leg_poly2);
    hip_joints[1] = transformPoint(leg1_joint_offset,leg_T1);
    hip_joints[1] = transformPoint(hip_joints[1],torso_T);
    drawCircle(ctx,hip_joints[1][0],hip_joints[1][1],hip_joint_r);
    
    //finally draw the foot
    //first initialize the offset and transformation
    var foot_poly1 = [];
    for(var i = 0; i < foot_poly.length; i++){
        foot_poly1[i] = foot_poly[i]; // copy of the foot
    }
    //define the offset
    var foot_offset = [-40,40];
    var foot_offset_1 = [-40,40];
    var foot_joint_offset = [40,0];
    var foot_joint_offset_1 = [40,0];
    
    //the foot need to rotate around the ankle, then bring it to the leg coordinate
    var foot_T = composeTransforms(
        rotationAboutPoint(ankle_angles[0],foot_joint_offset),
        translateByOffset(foot_offset)
    );
    var foot_T1 = composeTransforms(
        rotationAboutPoint(ankle_angles[1],foot_joint_offset_1),
        translateByOffset(foot_offset_1)
    );
    //draw the first feet, feet-> leg -> body -> world
    // this is the same kinematic tree structure as above all transformation
    foot_poly = transformPolygon(foot_poly,foot_T);
    foot_poly = transformPolygon(foot_poly,leg_T0);
    foot_poly = transformPolygon(foot_poly,torso_T);
    drawPolygon(ctx, foot_poly);
    // draw the first feet joint, joint->leg->body->world
    ankle_joints[0] = transformPoint(foot_joint_offset,foot_T);
    ankle_joints[0] = transformPoint(ankle_joints[0],leg_T0);
    ankle_joints[0] = transformPoint(ankle_joints[0],torso_T);
    drawCircle(ctx,ankle_joints[0][0],ankle_joints[0][1],ankle_joint_r);
    //draw the second feet, same as the first one
    foot_poly1 = transformPolygon(foot_poly1,foot_T1);
    foot_poly1 = transformPolygon(foot_poly1,leg_T1);
    foot_poly1 = transformPolygon(foot_poly1,torso_T);
    drawPolygon(ctx, foot_poly1);
    //draw the second joint, same as the first one
    //finish the whole penguin
    ankle_joints[1] = transformPoint(foot_joint_offset_1,foot_T1);
    ankle_joints[1] = transformPoint(ankle_joints[1],leg_T1);
    ankle_joints[1] = transformPoint(ankle_joints[1],torso_T);
    drawCircle(ctx,ankle_joints[1][0],ankle_joints[1][1],ankle_joint_r);

}


/**        BOILERPLATE DRAWING CODE        **/
/**   You do not need to modify this code  **/
function drawAxis(ctx)
{
  drawLineSegment(ctx, 0, 0, 100, 0, "red");
  drawLineSegment(ctx, 0, 0, 0, 100, "green");
  ctx.fillText("0,0", 4, 12);
  ctx.fillText("100,0", 98, 10);
  ctx.fillText("0,100", 5, 100);
}

// This function is called to draw the current frame. It is called whenever the
// sliders are changed.
function draw()
{
  // you do not need to modify this function
  // this is help function
  // Boilerplate code to setup the canvas
  var canvas = document.getElementById('canvas');
  if (!canvas.getContext)
  {
    alert("Your browser is too old! Please upgrade to a canvas-capable browser.");
  }
  var ctx = canvas.getContext('2d');
  // clear canvas so we can draw a new frame
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  // set global line width and smooth lines
  ctx.lineWidth = 2;
  ctx.lineJoin = 'round';
  ctx.lineCap = 'round';
  // draw a small axis to demonstrate the inverted coordinate system
  drawAxis(ctx);
  // draw the current Penguin
  drawPenguin(ctx);
}
/**     END OF BOILERPLATE DRAWING CODE       **/
