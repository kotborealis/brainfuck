const text = process.argv[2];

let prev_c = 0;

const bf = text.split('').map(c => c.charCodeAt(0)).map(c => {
    const diff = c - prev_c;
    let res;
    if(diff > 0){
        res ='+'.repeat(diff);
    }
    else if(diff < 0){
        res = '-'.repeat(diff * -1);
    }
    prev_c = c;
    return res;
}).join('.') + '.';

process.stdout.write(bf);
