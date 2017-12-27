console.assert(1!==2, "error: equal ");

var j = 0;
for(var i = 0;i < 10;i++){
  j +=i ;
}
console.assert(j===45, "error: for");

if(0){
  console.assert(false, "error: if");
}
if(1){
  
}else{
  console.assert(false, "error: else");
}

var value = 1 ? 3: 4;
console.assert(value=== 3, "error: ternary");

switch("a"){
  case "a":
    value = true;
    break;
  case "b":
  default:
    value = false;
    break;
}
console.assert(value, "error: switch");

function add(a,b){
  var c = a+b;
  return c
}
console.assert(add(1, 2) === 3, "error: function");

console.assert(1 <= 3, "error: <=");
console.assert(3 >= 1, "error:  >=");

console.assert('a'+1==='a1', "error: string + number")

console.assert("abc".indexOf('c') === 2, "error: String.indexOf")

console.assert(Math.abs(-7.25) === 7.25, "error: Math.abs")

console.assert(Math.acos(0.64) === 0.876298, "error: Math.acos")

console.assert(Math.asin(0.64) === 0.694498, "error: Math.asin")

console.assert(Math.atan(0.50) === 0.463647, "error: Math.atan")

console.assert(Math.ceil(0.60) === 1, "error: Math.ceil")
console.assert(Math.ceil(-5.1) === -5, "error: Math.ceil")

console.assert(Math.cos(3) === -0.989992, "error: Math.cos")

console.assert(Math.exp(1) === 2.718281, "error: Math.exp")

console.assert(Math.floor(0.6) === 0, "error: Math.floor")

console.assert(Math.log(2) === 0.693147, "error: Math.log")

console.assert(Math.max(5,7) === 7, "error: Math.max")

console.assert(Math.min(5,7) === 5, "error: Math.min")

console.assert(Math.pow(2,3) === 8, "error: Math.pow")

var random1 = Math.random();
var random2 = Math.random();

console.assert(random1 !== random2, "error: Math.random")

console.assert(Math.round(0.6) === 1, "error: Math.round")

console.assert(Math.sin(3) === 0.141120, "error: Math.sin")

console.assert(Math.sqrt(0.64) === 0.8, "error: Math.sqrt")

console.assert(Math.tan(0.5) === 0.546302, "error: Math.tan")

console.log("success")
