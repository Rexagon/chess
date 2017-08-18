var net = require('net');

var HOST = "127.0.0.1";
var PORT = 49876;

var sendPackets = {
	signIn1: {
		command: 0,
		data: ["test", "1234"],
		comment: "Sign in \n{\n    test,\n    1234\n}\n",
		callback: function() {

		}
	},
	signUp: {
		command: 1,
		data: ["test10", "1234"],
		comment: "Sign up user \n{\n    test10,\n    1234\n}\n",
		callback: function() {

		}
	},
	signOut: {
		command: 2,
		data: [],
		comment: "Sign out",
		callback: function() {

		}
	},
	signIn2: {
		command: 0,
		data: ["test10", "1234"],
		comment: "Sign in \n{\n    test10,\n    1234\n}\n",
		callback: function() {

		}
	},
	changeRoom1: {
		command: 8,
		data: ["test_room_1"],
		comment: "Change room \n{\n    test_room_1\n}\n",
		callback: function() {

		}
	},
	changeRoom2: {
		command: 8,
		data: ["test_room_2"],
		comment: "Change room \n{\n    test_room_2\n}\n",
		callback: function() {

		}
	},
	changeRoom3: {
		command: 8,
		data: ["test_room_3"],
		comment: "Change room \n{\n    test_room_3\n}\n",
		callback: function() {

		}
	},
	leaveFromRoom: {
		command: 8,
		data: [],
		comment: "Leave from room",
		callback: function() {

		}
	},
	roomList: {
		command: 5,
		data: [],
		comment: "Require room list",
		callback: function() {

		}
	},
	getRoomSettings1: {
		command: 10,
		data: ["test_room_1"],
		comment: "Require room settings \n{\n    test_room_1\n}\n",
		callback: function() {

		}
	},
	createRoom: {
		command: 10,
		data: [
			"room_test_10",
			"room_test_10",
			"test10",
			"test1",
			"test2",
			"0",
			"1"
		],
		comment: "Create room \n{\n    test_room_10,\n    room_test_10,\n    test10,\n    test1,\n    test2,\n    0,\n    1\n}\n",
		callback: function() {

		}
	},
	getRoomSettings2: {
		command: 10,
		data: ["room_test_10"],
		comment: "Require room settings \n{\n    test_room_10\n}\n",
		callback: function() {

		}
	},
	setRoomSettings: {
		command: 10,
		data: [
			"room_test_10",
			"room_test_50",
			"test10",
			"test2",
			"test3",
			"1",
			"0"
		],
		comment: "Set room settings \n{\n    test_room_10,\n    room_test_50,\n    test10,\n    test2,\n    test3,\n    1,\n    0\n}\n",
		callback: function() {

		}
	},
	sendMessage: {
		command: 9,
		data: [],
		comment: "",
		callback: function() {

		}
	}
}

function makePacket(command, data) {
	var length = 0;
	for (var i = 0; i < data.length; i++) {
		length += (data[i].length * 4);
	}
	length += 1;

	var buff = new Buffer(length);
	buff.writeInt8(command, 0);
	var pos = 1;
	for (var i = 0; i < data.length; i++) {
		buff.writeUInt32BE(data[i].length * 4);
		for (var k = 0; k < data[i].length; k++) {
			buff.writeUInt32BE(data[i][k].charCodeAt(0));
		}
	}
	return buff;
}

var client = new net.Socket();

client.connect(PORT, HOST, function() {
	console.log("Connected");
	client.write('kek');
	//makePacket(sendPackets.signIn1.command, sendPackets.signIn1.data)
});

client.on("data", function(data) {
	const buf = Buffer.from(data);
	console.log(buf);
});

function doIt() {
	for (var packet in packets) {
		if (packets.hasOwnProperty(packet)) {

		}
	}
}

