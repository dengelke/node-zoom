extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>
#include <nan.h>
#include "connection.h"
#include "scanset.h"
#include "query.h"
#include "resultset.h"
#include "record.h"

using namespace v8;

Handle<Value> CreateConnection(const Arguments& args) {
	HandleScope scope;
	return scope.Close(ConnectionObject::NewInstance(args));
}

void initAll(Handle<Object> target){

	ConnectionObject::Init();
	ScanSetObject::Init();
	QueryObject::Init();
	ResultSetObject::Init();
	RecordObject::Init();

	Handle<ObjectTemplate> Connection = ObjectTemplate::New();

	Connection->Set(Nan::New("create").ToLocalChecked(), FunctionTemplate::New(CreateConnection)->GetFunction());

	target->Set(Nan::New("connection").ToLocalChecked(), Connection->NewInstance());
	target->Set(Nan::New("query").ToLocalChecked(), QueryObject::NewInstance());
  target->Set(Nan::New("scanset").ToLocalChecked(), ScanSetObject::NewInstance());
  target->Set(Nan::New("resultset").ToLocalChecked(), ResultSetObject::NewInstance());
	target->Set(Nan::New("record").ToLocalChecked(), RecordObject::NewInstance());
}

NODE_MODULE(zoom, initAll);
