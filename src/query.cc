extern "C"{
	#include <yaz/zoom.h>
}
#include <node.h>
#include <nan.h>
#include "connection.h"
#include "query.h"

using namespace v8;

bool query_failed(int g, char * query, Handle<Value> exception){
	if (g == -1){
		exception = ThrowException(Exception::Error(Nan::New("Query Error").ToLocalChecked()));
		return true;
	}
	return false;
}

QueryObject::QueryObject(){};
QueryObject::~QueryObject(){};

Persistent<Function> QueryObject::constructor;

void QueryObject::Init(){
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(Nan::New("Query").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	tpl->PrototypeTemplate()->Set(Nan::New("destroy").ToLocalChecked(), 
			FunctionTemplate::New(destroy)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(Nan::New("cql").ToLocalChecked(), 
			FunctionTemplate::New(cql)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(Nan::New("cql2rpn").ToLocalChecked(), 
			FunctionTemplate::New(cql2rpn)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(Nan::New("prefix").ToLocalChecked(), 
			FunctionTemplate::New(prefix)->GetFunction());
	
	tpl->PrototypeTemplate()->Set(Nan::New("sortby").ToLocalChecked(), 
			FunctionTemplate::New(sortby)->GetFunction());
	
	constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> QueryObject::New(const Arguments& args){
	HandleScope scope;
	QueryObject * query = new QueryObject();
	query->Wrap(args.This());
	return args.This();
}

Handle<Value> QueryObject::NewInstance(const Arguments& args){
	HandleScope scope;
	
	Local<Object> instance = constructor->NewInstance();

	QueryObject * obj = node::ObjectWrap::Unwrap<QueryObject>(instance);
	
	obj->q = ZOOM_query_create();

	return scope.Close(instance);
}

Handle<Value> QueryObject::NewInstance(){
	HandleScope scope;
	
	Local<Object> instance = constructor->NewInstance();

	return scope.Close(instance);
}

Handle<Value> QueryObject::destroy(const Arguments& args){
	HandleScope scope;
	QueryObject * obj = node::ObjectWrap::Unwrap<QueryObject>(args.This());
	ZOOM_query_destroy(obj->q);
	return args.This();
}

Handle<Value> QueryObject::cql(const Arguments& args){
	HandleScope scope;
	ZOOM_query query = ZOOM_query_create();
	Handle<Value> exception;
	String::Utf8Value str(args[0]);
	Local<Object> instance = constructor->NewInstance();
	QueryObject * obj = node::ObjectWrap::Unwrap<QueryObject>(instance);
	
	if(query_failed(ZOOM_query_cql(query, *str), *str, exception)){
		return exception;
	}
	
	obj->q = query;
	return scope.Close(instance);
}

Handle<Value> QueryObject::cql2rpn(const Arguments& args){
	HandleScope scope;
	ZOOM_query query = ZOOM_query_create();
	ConnectionObject * cobj = node::ObjectWrap::Unwrap<ConnectionObject>(args[1]->ToObject());
	Handle<Value> exception;
	String::Utf8Value str(args[0]);
	Local<Object> instance = constructor->NewInstance();
	QueryObject * obj = node::ObjectWrap::Unwrap<QueryObject>(instance);
	
	if(query_failed(ZOOM_query_cql2rpn(query, *str, cobj->conn), *str, exception)){
		return exception;
	}
	
	obj->q = query;
	return scope.Close(instance);
}

Handle<Value> QueryObject::prefix(const Arguments& args){
	HandleScope scope;
	ZOOM_query query = ZOOM_query_create();
	Handle<Value> exception;
	String::Utf8Value str(args[0]);
	Local<Object> instance = constructor->NewInstance();
	QueryObject * obj = node::ObjectWrap::Unwrap<QueryObject>(instance);
	
	if(query_failed(ZOOM_query_prefix(query, *str), *str, exception)){
		return exception;
	}
	
	obj->q = query;
	return scope.Close(instance);
}

Handle<Value> QueryObject::sortby(const Arguments& args){
	HandleScope scope;
	ZOOM_query query = ZOOM_query_create();
	QueryObject * obj = new QueryObject();
	String::Utf8Value str(args[0]);
	
	ZOOM_query_sortby(query, *str); 
	
	obj->q = query;
	obj->Wrap(args.This());
	return args.This();
}
