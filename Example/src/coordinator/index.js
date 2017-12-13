
var CoordinatorContext = function (sponsorAffinity, responderAffinity, action) {

	if (!(this instanceof CoordinatorContext)) {
		return new CoordinatorContext(sponsorAffinity, responderAffinity, action);
	}
  if (sponsorAffinity === null || sponsorAffinity === undefined
    || !(typeof sponsorAffinity == 'string') || sponsorAffinity.length == 0) {
      throw new Error('sponsorAffinity must be defined as not empty String');
  }

  if (responderAffinity === undefined || responderAffinity === null
    || !(typeof responderAffinity == 'string') || responderAffinity.length == 0) {
      throw new Error('responderAffinity must be defined as not empty String');
  }

  if (action === undefined || action === null
    || !(typeof action == 'string') || responderAffinity.action == 0) {
      throw new Error('action must be defined as not empty String');
  }

	this.responderAffinity = responderAffinity;
	this.sponsorAffinity = sponsorAffinity;
  this.action = action;

  // Native method
  LynxCoordinatorRegister.registerAction(sponsorAffinity,
     responderAffinity, action);

};

CoordinatorContext.prototype = {
	sponsorAffinity: this.sponsorAffinity,
	responderAffinity: this.responderAffinity,
	action: this.action,

	toString: function () {
		return "sponsor:" + this.sponsorAffinity + " responder:"
            + this.responderAffinity + " action:" + this.action;
	}
};

CoordinatorContext.prototype.updateProperties = function (properties, notify = false) {

	if (typeof properties !== 'object') {
		throw new Error('properties must be defined as Object');
	}

  LynxCoordinatorRegister.updateProperties(this.sponsorAffinity,
                                           this.responderAffinity,
                                           properties,
                                           notify);

};

export default CoordinatorContext;
