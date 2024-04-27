; ModuleID = 'dynamic_AoS.c'
source_filename = "dynamic_AoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeOne = type { i32, i8, double }
%struct.nodeTwo = type { float, double }

@globalFour = dso_local global ptr null, align 8
@globalThree = dso_local global ptr null, align 8
@globalFive = dso_local global ptr null, align 8
@globalOne = dso_local global ptr null, align 8
@globalTwo = dso_local global ptr null, align 8
@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@globalSix = dso_local global ptr null, align 8
@invalidGlobal = dso_local global %struct.nodeOne zeroinitializer, align 8

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOne(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  store i32 22, ptr %a, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %4 = load i32, ptr %i, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeTwo(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 0
  store float 1.200000e+01, ptr %a, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %4 = load i32, ptr %i, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !8

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateThree() #0 {
entry:
  %call = call noalias ptr @calloc(i64 noundef 1000, i64 noundef 16) #4
  store ptr %call, ptr @globalFour, align 8
  ret void
}

; Function Attrs: nounwind allocsize(0,1)
declare noalias ptr @calloc(i64 noundef, i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateTwo() #0 {
entry:
  %0 = load ptr, ptr @globalFour, align 8
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %0, i64 67
  %b = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 1
  store double 9.023000e+01, ptr %b, align 8
  call void @populateThree()
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populate() #0 {
entry:
  %f = alloca i32, align 4
  %0 = load ptr, ptr @globalThree, align 8
  call void @populateNodeOne(ptr noundef %0, i32 noundef 100)
  store i32 100, ptr %f, align 4
  %1 = load i32, ptr %f, align 4
  %conv = sext i32 %1 to i64
  %call = call noalias ptr @calloc(i64 noundef %conv, i64 noundef 16) #4
  store ptr %call, ptr @globalFour, align 8
  call void @populateTwo()
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateParam0(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  %0 = load ptr, ptr %array.addr, align 8
  %1 = load i32, ptr %n.addr, align 4
  call void @populateParam1(ptr noundef %0, i32 noundef %1)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateParam1(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  %0 = load i32, ptr %n.addr, align 4
  %cmp = icmp sgt i32 %0, 100
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load i32, ptr %n.addr, align 4
  %dec = add nsw i32 %1, -1
  store i32 %dec, ptr %n.addr, align 4
  br label %if.end

if.else:                                          ; preds = %entry
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %n.addr, align 4
  call void @populateParam2(ptr noundef %2, i32 noundef %3)
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateParam2(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  %innerArrayOne = alloca ptr, align 8
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  %call = call noalias ptr @malloc(i64 noundef 1600) #5
  store ptr %call, ptr %innerArrayOne, align 8
  %0 = load ptr, ptr %array.addr, align 8
  %1 = load i32, ptr %n.addr, align 4
  call void @populateParam3(ptr noundef %0, i32 noundef %1)
  %2 = load ptr, ptr %innerArrayOne, align 8
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %2, i64 0
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  store i32 99, ptr %a, align 8
  ret void
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateParam3(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  %0 = load ptr, ptr %array.addr, align 8
  %1 = load i32, ptr %n.addr, align 4
  call void @populateParam4(ptr noundef %0, i32 noundef %1)
  %call = call noalias ptr @malloc(i64 noundef 1600) #5
  store ptr %call, ptr @globalFive, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateParam4(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  %arrayTwo = alloca ptr, align 8
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  %0 = load ptr, ptr %array.addr, align 8
  %1 = load i32, ptr %n.addr, align 4
  call void @populateNodeOne(ptr noundef %0, i32 noundef %1)
  %call = call noalias ptr @calloc(i64 noundef 100, i64 noundef 16) #4
  store ptr %call, ptr %arrayTwo, align 8
  %2 = load ptr, ptr %arrayTwo, align 8
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %2, i64 23
  %a = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 0
  store float 0x3FF051EB80000000, ptr %a, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateGlobal(i32 noundef %n, ptr noundef %array) #0 {
entry:
  %n.addr = alloca i32, align 4
  %array.addr = alloca ptr, align 8
  store i32 %n, ptr %n.addr, align 4
  store ptr %array, ptr %array.addr, align 8
  %0 = load i32, ptr %n.addr, align 4
  %conv = sext i32 %0 to i64
  %mul = mul i64 %conv, 16
  %call = call noalias ptr @malloc(i64 noundef %mul) #5
  store ptr %call, ptr %array.addr, align 8
  %1 = load ptr, ptr %array.addr, align 8
  %2 = load i32, ptr %n.addr, align 4
  call void @populateNodeOne(ptr noundef %1, i32 noundef %2)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @returnAoS(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %temp = alloca ptr, align 8
  store i32 %size, ptr %size.addr, align 4
  %0 = load i32, ptr %size.addr, align 4
  %conv = sext i32 %0 to i64
  %mul = mul i64 %conv, 16
  %call = call noalias ptr @malloc(i64 noundef %mul) #5
  store ptr %call, ptr %temp, align 8
  %1 = load ptr, ptr %temp, align 8
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %1, i64 100
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  store i32 90, ptr %a, align 8
  %2 = load ptr, ptr %temp, align 8
  ret ptr %2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @returnAoSTwo(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  %temp = alloca ptr, align 8
  %tempTwo = alloca ptr, align 8
  store i32 %size, ptr %size.addr, align 4
  %0 = load i32, ptr %size.addr, align 4
  %conv = sext i32 %0 to i64
  %mul = mul i64 %conv, 16
  %call = call noalias ptr @malloc(i64 noundef %mul) #5
  store ptr %call, ptr %temp, align 8
  %1 = load ptr, ptr %temp, align 8
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %1, i64 76
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  store i32 54, ptr %a, align 8
  %2 = load ptr, ptr %temp, align 8
  store ptr %2, ptr %tempTwo, align 8
  %3 = load ptr, ptr %tempTwo, align 8
  ret ptr %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @returnAoSOne(i32 noundef %size) #0 {
entry:
  %size.addr = alloca i32, align 4
  store i32 %size, ptr %size.addr, align 4
  %0 = load i32, ptr %size.addr, align 4
  %call = call ptr @returnAoSTwo(i32 noundef %0)
  ret ptr %call
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %arrayOne = alloca ptr, align 8
  %b = alloca i32, align 4
  %arrayTwo = alloca ptr, align 8
  %c = alloca i32, align 4
  %intArray = alloca ptr, align 8
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  %arrayThree = alloca ptr, align 8
  %f = alloca i32, align 4
  %returnedOne = alloca ptr, align 8
  %returnedTwo = alloca ptr, align 8
  %returnThree = alloca ptr, align 8
  store i32 0, ptr %retval, align 4
  store i32 100, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %conv = sext i32 %0 to i64
  %mul = mul i64 %conv, 16
  %call = call noalias ptr @malloc(i64 noundef %mul) #5
  store ptr %call, ptr %arrayOne, align 8
  %1 = load ptr, ptr %arrayOne, align 8
  %2 = load i32, ptr %a, align 4
  call void @populateNodeOne(ptr noundef %1, i32 noundef %2)
  store i32 100, ptr %b, align 4
  %3 = load i32, ptr %b, align 4
  %conv1 = sext i32 %3 to i64
  %call2 = call noalias ptr @calloc(i64 noundef %conv1, i64 noundef 16) #4
  store ptr %call2, ptr %arrayTwo, align 8
  %4 = load ptr, ptr %arrayTwo, align 8
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %4, i64 23
  %a3 = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 0
  store float 0x3FF051EB80000000, ptr %a3, align 8
  store i32 100, ptr %c, align 4
  %5 = load i32, ptr %c, align 4
  %conv4 = sext i32 %5 to i64
  %mul5 = mul i64 %conv4, 16
  %call6 = call noalias ptr @malloc(i64 noundef %mul5) #5
  store ptr %call6, ptr @globalOne, align 8
  %6 = load ptr, ptr @globalOne, align 8
  %arrayidx7 = getelementptr inbounds %struct.nodeOne, ptr %6, i64 4
  %b8 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx7, i32 0, i32 1
  store i8 118, ptr %b8, align 4
  %7 = load i32, ptr %c, align 4
  %conv9 = sext i32 %7 to i64
  %call10 = call noalias ptr @calloc(i64 noundef %conv9, i64 noundef 4) #4
  store ptr %call10, ptr %intArray, align 8
  %8 = load ptr, ptr %intArray, align 8
  %arrayidx11 = getelementptr inbounds i32, ptr %8, i64 55
  store i32 23525, ptr %arrayidx11, align 4
  %9 = load i32, ptr %c, align 4
  %conv12 = sext i32 %9 to i64
  %mul13 = mul i64 %conv12, 16
  %call14 = call noalias ptr @malloc(i64 noundef %mul13) #5
  store ptr %call14, ptr @globalOne, align 8
  store i32 100, ptr %d, align 4
  %10 = load i32, ptr %d, align 4
  %conv15 = sext i32 %10 to i64
  %call16 = call noalias ptr @calloc(i64 noundef %conv15, i64 noundef 16) #4
  store ptr %call16, ptr @globalTwo, align 8
  %11 = load ptr, ptr @globalTwo, align 8
  %12 = load i32, ptr %d, align 4
  call void @populateNodeTwo(ptr noundef %11, i32 noundef %12)
  store i32 100, ptr %e, align 4
  %13 = load i32, ptr %e, align 4
  %conv17 = sext i32 %13 to i64
  %mul18 = mul i64 %conv17, 16
  %call19 = call noalias ptr @malloc(i64 noundef %mul18) #5
  store ptr %call19, ptr %arrayThree, align 8
  store i32 100, ptr %f, align 4
  %14 = load i32, ptr %e, align 4
  %conv20 = sext i32 %14 to i64
  %mul21 = mul i64 %conv20, 16
  %call22 = call noalias ptr @malloc(i64 noundef %mul21) #5
  store ptr %call22, ptr @globalThree, align 8
  call void @populate()
  %15 = load ptr, ptr %arrayThree, align 8
  %16 = load i32, ptr %e, align 4
  call void @populateParam0(ptr noundef %15, i32 noundef %16)
  %call23 = call ptr @returnAoS(i32 noundef 100)
  store ptr %call23, ptr %returnedOne, align 8
  %17 = load ptr, ptr %returnedOne, align 8
  %arrayidx24 = getelementptr inbounds %struct.nodeOne, ptr %17, i64 10
  %a25 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx24, i32 0, i32 0
  store i32 100, ptr %a25, align 8
  %18 = load ptr, ptr %returnedOne, align 8
  %arrayidx26 = getelementptr inbounds %struct.nodeOne, ptr %18, i64 10
  %a27 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx26, i32 0, i32 0
  %19 = load i32, ptr %a27, align 8
  %call28 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %19)
  %20 = load ptr, ptr %returnedOne, align 8
  store ptr %20, ptr %returnedTwo, align 8
  %21 = load ptr, ptr %returnedOne, align 8
  %arrayidx29 = getelementptr inbounds %struct.nodeOne, ptr %21, i64 10
  %a30 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx29, i32 0, i32 0
  store i32 77, ptr %a30, align 8
  %22 = load ptr, ptr %returnedTwo, align 8
  %arrayidx31 = getelementptr inbounds %struct.nodeOne, ptr %22, i64 10
  %a32 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx31, i32 0, i32 0
  %23 = load i32, ptr %a32, align 8
  %call33 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %23)
  %24 = load ptr, ptr %returnedTwo, align 8
  %arrayidx34 = getelementptr inbounds %struct.nodeOne, ptr %24, i64 6
  %b35 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx34, i32 0, i32 1
  store i8 99, ptr %b35, align 4
  %25 = load ptr, ptr %returnedOne, align 8
  %arrayidx36 = getelementptr inbounds %struct.nodeOne, ptr %25, i64 6
  %b37 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx36, i32 0, i32 1
  %26 = load i8, ptr %b37, align 4
  %conv38 = sext i8 %26 to i32
  %call39 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %conv38)
  %call40 = call ptr @returnAoSOne(i32 noundef 99)
  store ptr %call40, ptr %returnThree, align 8
  %27 = load ptr, ptr @globalFive, align 8
  %arrayidx41 = getelementptr inbounds %struct.nodeOne, ptr %27, i64 1
  %a42 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx41, i32 0, i32 0
  store i32 100, ptr %a42, align 8
  %28 = load ptr, ptr @globalFive, align 8
  %arrayidx43 = getelementptr inbounds %struct.nodeOne, ptr %28, i64 1
  %a44 = getelementptr inbounds %struct.nodeOne, ptr %arrayidx43, i32 0, i32 0
  %29 = load i32, ptr %a44, align 8
  %call45 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %29)
  %30 = load i32, ptr %d, align 4
  %31 = load ptr, ptr @globalSix, align 8
  call void @populateGlobal(i32 noundef %30, ptr noundef %31)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #3

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0,1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind allocsize(0,1) }
attributes #5 = { nounwind allocsize(0) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 893416051d517d979481510fe3b52d83ba0d1e01)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
