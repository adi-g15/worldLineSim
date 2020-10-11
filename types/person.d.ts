export class Person{
    name: string;
    age?: number;

    constructor(name: string, age?: number){
        this.name = name;
        if( age ) this.age = age;
    }
}
